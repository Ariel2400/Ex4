#pragma once

#include "client_handler.hpp"
#include "graph_client_handler.hpp"
#include "A-star_matrix_searcher.hpp"
#include "BestFS_matrix_searcher.hpp"
#include "BFS_matrix_searcher.hpp"
#include "DFS_matrix_searcher.hpp"

class Server {
    public:
        // opens the server's port
        virtual void open(int port) = 0;
        
        // closes the server
        virtual void stop() = 0;

        // destructor
        virtual ~Server() = default;
};