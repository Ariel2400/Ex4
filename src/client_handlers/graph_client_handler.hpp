#pragma once

#include <map>
#include <memory>

#include "client_handler.hpp"
#include "A-star_matrix_searcher.hpp"
#include "BestFS_matrix_searcher.hpp"
#include "BFS_matrix_searcher.hpp"
#include "DFS_matrix_searcher.hpp"

class GraphClientHandler: public ClientHandler{
    public:
        // constructor
        GraphClientHandler();

        // copy constructor
        GraphClientHandler(GraphClientHandler &handler);

        /* reads matrix description and problem input from the client, uses the given algorithm
           to solve it and sends the solution to the client */
        void virtual handle_client(int client_socket, std::string algorithm);

        // destructor
        ~GraphClientHandler();

    private:
        // a map that matches a matrix search algorithm to its name
        std::map<std::string, std::shared_ptr<MatrixSearcher>> algorithms;

        // sends a confirmation that the solve command was received
        void send_confirmation(int client_socket);

        // sends a response that includes the search status and the solution if it was found
        void send_response(int client_socket, SearchStatus status, std::string path, double weight, std::string algorithm);
};