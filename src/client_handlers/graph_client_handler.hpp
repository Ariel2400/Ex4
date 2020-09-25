#include "client_handler.hpp"

class GraphClientHandler: public ClientHandler{
    private:
        std::unique_ptr<MatrixSearcher> algorithm;
    public:
        GraphClientHandler(std::unique_ptr<MatrixSearcher> &searcher);
        GraphClientHandler(GraphClientHandler &handler);
        ~GraphClientHandler();
        void handle_client(int cs, int ss) override;
};