#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string>
#include <regex>
#include <iostream>

#include "parallel_server.hpp"
#include "client_redirector.hpp"

#define BUFFER_SIZE 4096
#define SERVER_BACKLOG 20
#define NO_SOCKET (-1)
#define CLIENT_TIMEOUT_SECONDS 5

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

ParallelServer::ParallelServer() {
    is_server_open = false;
}

void ParallelServer::look_for_clients(struct sockaddr_in address) {
    int new_socket;
    int addrlen = sizeof(address);
    while (is_server_open) {
        if ((new_socket = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen))) < 0) {
            throw std::system_error(EDOM, std::generic_category(), "connection failed");
        }
        // printing the new client collection to the log
        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << "client connected at " << ctime(&timenow) << " through socket " << new_socket << std::endl;
        std::unique_lock<std::mutex> lock(client_queue_mutex);
        client_queue.push(new_socket);
        lock.unlock();
    }
}

void ParallelServer::attend_client_queue() {
    while (is_server_open) {
        int client_socket = NO_SOCKET;
        std::unique_lock<std::mutex> lock(client_queue_mutex);
        if (!client_queue.empty()) {
            client_socket = client_queue.front();
            client_queue.pop();
        }
        lock.unlock();
        if (client_socket != NO_SOCKET) {
            bool is_active = false;
            bool is_checking = true;
            std::thread client_input(&ParallelServer::read_from_client, this, client_socket, &is_active);
            std::thread activity_check(&ParallelServer::check_for_activity, this, &is_checking);
            while (!is_active && is_checking) {
                std::this_thread::yield();
            }
            if (is_active) {
                client_input.join();
            } else {
                client_input.detach();
            }
            activity_check.detach();
            close(client_socket);
        }
    }
}

void ParallelServer::read_from_client(int client_socket, bool* is_active) {
    char buffer[BUFFER_SIZE] = {0}; 
    read(client_socket, buffer, BUFFER_SIZE);
    *is_active = true;
    std::string message(buffer);
    ClientRedirector client_redirector;
    client_redirector.transfer_client(message, client_socket);
    close(client_socket);
}

void ParallelServer::open(int port) {
    struct sockaddr_in address; 
    int opt = 1; 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        throw std::system_error(EDOM, std::generic_category(), "socket failed");
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
        throw std::system_error(EDOM, std::generic_category(), "setsockopt failed"); 
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address))<0) {
        throw std::system_error(EDOM, std::generic_category(), "bind failed");
    }
    if (listen(server_fd, SERVER_BACKLOG) < 0) {
        throw std::system_error(EDOM, std::generic_category(), "listen failed");
    }
    client_handling_loop = std::make_unique<std::thread>(&ParallelServer::look_for_clients, this, address);
    is_server_open = true;
    for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
        thread_pool[i] = std::make_shared<std::thread>(&ParallelServer::attend_client_queue, this);
    }
    client_handling_loop->join();
    for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
        thread_pool[i]->join();
    }
}

void ParallelServer::check_for_activity(bool* is_checking) {
    std::this_thread::sleep_for(std::chrono::seconds(CLIENT_TIMEOUT_SECONDS));
    *is_checking = false;
}

void ParallelServer::stop() {
    client_handling_loop->detach();
    for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
        thread_pool[i]->detach();
    }
    close(server_fd);
    is_server_open = false;
}

ParallelServer::~ParallelServer() {
    stop();
}