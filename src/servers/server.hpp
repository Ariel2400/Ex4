#include "../client_handlers/client_handler.cpp"

class Server {
    public:
        virtual void open(int port, ClientHandler const &c) = 0;
        virtual void stop() = 0;
};