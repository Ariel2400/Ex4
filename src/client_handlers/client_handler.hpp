#pragma once

#include <memory>

#include "matrix_searcher.hpp"

class ClientHandler {
    public:
        /* reads problem input from the client, uses the given algorithm
           to solve it and sends the solution to the client */
        void virtual handle_client(int client_socket, std::string algorithm) = 0;  

        // destructor
        virtual ~ClientHandler() = default;     
};