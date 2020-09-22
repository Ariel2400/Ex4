#include "../client_handlers/client_handler.cpp"

class Server {
    public:
        virtual void open(int port) = 0;
        virtual void stop() = 0;
};