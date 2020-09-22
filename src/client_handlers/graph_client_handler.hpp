#include "client_handler.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include "../src/matrices/matrix_class.hpp"
#include "../src/algorithms/matrix.hpp"
#define BUFFER 1024

class GraphClientHandler:ClientHandler{
    public:
        void handleClient(int cs, int ss) override;
}