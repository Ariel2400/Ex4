#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <regex>
#include <iostream>
#include <thread>
#include <pthread.h>

#include "serial_server.hpp"


typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

#define BUFFER_SIZE 4096
#define SERVER_BACKLOG 20
#define SECONDS_TO_DISCONNECTION 5

SerialServer::SerialServer() {
    is_server_open = false;
}

bool SerialServer::is_valid_solve_command(std::string line, std::vector<std::string>* solve_params) {
    std::vector<std::vector<std::string>> algorithms;
    std::vector<std::string> find_graph_path_algorithms({"find-graph-path", "[dfs]", "[bfs]", "[bestfs]", "[a*]"});
    algorithms.push_back(find_graph_path_algorithms);
    std::regex whitespace("\\s+");
    std::sregex_token_iterator iter(line.begin(), line.end(), whitespace, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> params;
    while(iter != end) {
        std::string param(*iter++);
        std::for_each(param.begin(), param.end(), [](char & c){
            c = ::tolower(c);
        });
        params.push_back(param);
    }
    if (params.size() != 3) {
        return false;
    }
    if (params.at(0).compare("solve") != 0) {
        return false;
    }
    for (int i = 0; i < algorithms.size(); ++i) {
        if (params.at(1).compare(algorithms.at(i).at(0)) == 0) {
            for (int j = 1; j < algorithms.at(i).size(); ++j) {
                if (params.at(2).compare(algorithms.at(i).at(j)) == 0) {
                    *solve_params = params;
                    return true;
                }
            }
        }
    }
    return false;
}

void SerialServer::look_for_clients(struct sockaddr_in address) {
    int new_socket;
    int addrlen = sizeof(address); 
    while (is_server_open) {
        if ((new_socket = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen))) < 0) {
            throw std::system_error(EDOM, std::generic_category(), "connection failed");
        }
        transfer_client(new_socket);
    }
}

void SerialServer::transfer_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0}; 
    read(client_socket, buffer, BUFFER_SIZE);
    std::string command(buffer);
    std::vector<std::string> params;
    std::unique_ptr<ClientHandler> client_handler;
    std::unique_ptr<MatrixSearcher> matrix_searcher;
    if (is_valid_solve_command(command, &params)) {
        if (params.at(2).compare("[dfs]") == 0) {
            matrix_searcher = std::make_unique<DFSMatrixSearcher>();
        } else if (params.at(2).compare("[bfs]") == 0) {
            matrix_searcher = std::make_unique<BFSMatrixSearcher>();
        } else if (params.at(2).compare("[bestfs]") == 0) {
            matrix_searcher = std::make_unique<BestFSMatrixSearcher>();
        } else if (params.at(2).compare("[a*]") == 0) {
            matrix_searcher = std::make_unique<BFSMatrixSearcher>();
        } else {
            throw std::system_error(EDOM, std::generic_category(), "unknown algorithm: " + params.at(2));
        }
        if (params.at(1).compare("find-graph-path")  == 0) {
            client_handler = std::make_unique<GraphClientHandler>(matrix_searcher);
        } else {
            throw std::system_error(EDOM, std::generic_category(), "unknown problem: " + params.at(1));
        }
        client_handler->handle_client(client_socket, client_socket); 
    } else {
        throw std::system_error(EDOM, std::generic_category(), "not a valid command: " + command);
    }
    close(client_socket);
}

void SerialServer::open(int port) {
    struct sockaddr_in address; 
    int opt = 1; 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        throw std::system_error(EDOM, std::generic_category(), "socket failed");
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0) {
        throw std::system_error(EDOM, std::generic_category(), "bind failed");
    }
    if (listen(server_fd, SERVER_BACKLOG) < 0) {
        throw std::system_error(EDOM, std::generic_category(), "listen failed");
    }
    client_handling_loop = std::make_unique<std::thread>(&SerialServer::look_for_clients, this, address);
    is_server_open = true;
    client_handling_loop->join();
}

void SerialServer::stop() {
    client_handling_loop->detach();
    close(server_fd);
    is_server_open = false;
}

SerialServer::~SerialServer() {}