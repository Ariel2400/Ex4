#pragma once

#include <string>
#include <map>
#include <memory>

#include "graph_client_handler.hpp"

class ClientRedirector {
    public:
        // constructor
        ClientRedirector();

        /* recieves a message describing a problem and an algorithm and redirects the 
           client to the fitting client handler */
        void transfer_client(std::string message, int client_port);

        // destructor
        ~ClientRedirector();

    private:
        // a map that matches a client handler to its name
        std::map<std::string, std::shared_ptr<ClientHandler>> problems;
};