#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <regex>
#include <iostream>
#include <chrono>
#include <iostream>

#include "serial_server.hpp"
#include "client_redirector.hpp"

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

#define BUFFER_SIZE 4096
#define SERVER_BACKLOG 20
#define CLIENT_TIMEOUT_SECONDS 5

SerialServer::SerialServer() {
    is_server_open = false;
}

void SerialServer::open(int port) {
    struct sockaddr_in address; 
    int opt = 1; 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        throw std::system_error(EDOM, std::generic_category(), "socket failed");
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
        throw std::system_error(EDOM, std::generic_category(), "socket failed");
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
    client_handling_loop = std::make_unique<std::thread>(&SerialServer::look_for_clients, 
                                                            this, address);
    is_server_open = true;
    client_handling_loop->join();
}

void SerialServer::look_for_clients(struct sockaddr_in address) {
    int new_socket;
    int addrlen = sizeof(address); 
    while (is_server_open) {
        if ((new_socket = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), 
                                    reinterpret_cast<socklen_t*>(&addrlen))) < 0) {
            throw std::system_error(EDOM, std::generic_category(), "connection failed");
        }
        // printing the new client collection to the log
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << "client connected at " << ctime(&timenow) << " through socket " 
                    << new_socket << std::endl;
        bool is_active = false;
        bool is_checking = true;
        std::thread client_input(&SerialServer::read_from_client, this, new_socket, &is_active);
        std::thread activity_check(&SerialServer::check_for_activity, this, &is_checking);
        while (!is_active && is_checking) {
            std::this_thread::yield();
        }
        if (is_active) {
            client_input.join();
        } else {
            client_input.detach();
        }
        activity_check.detach();
        close(new_socket);
    }
}

void SerialServer::read_from_client(int client_socket, bool* is_active) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);
    *is_active = true;
    std::string message(buffer);
    ClientRedirector client_redirector;
    client_redirector.transfer_client(message, client_socket);
    close(client_socket);
}

void SerialServer::check_for_activity(bool* is_checking) {
    std::this_thread::sleep_for(std::chrono::seconds(CLIENT_TIMEOUT_SECONDS));
    *is_checking = false;
}

void SerialServer::stop() {
    client_handling_loop->detach();
    close(server_fd);
    is_server_open = false;
}

SerialServer::~SerialServer() {}