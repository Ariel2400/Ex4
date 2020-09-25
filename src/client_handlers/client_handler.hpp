#include "matrix_searcher.hpp"

class ClientHandler {
    public:
        void virtual handle_client(int cs, int ss) = 0;       
};