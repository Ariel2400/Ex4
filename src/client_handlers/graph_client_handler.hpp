#include "client_handler.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <regex>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include "../src/matrices/matrix_class.hpp"
#include "../src/algorithms/matrix_searcher.hpp"
#define BUFFER 1024
#define NONMEMBERLINES 4

class GraphClientHandler:ClientHandler{
    private:
        std::unique_ptr<MatrixSearcher> algorithm;
    public:
        GraphClientHandler(MatrixSearcher *searcher);
        GraphClientHandler(GraphClientHandler &handler);
        ~GraphClientHandler();
        void handleClient(int cs, int ss) override;
};