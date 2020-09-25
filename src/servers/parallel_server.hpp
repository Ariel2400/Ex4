#include "server.hpp"
#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>

#define THREAD_POOL_SIZE 20

class ParallelServer: public Server
{
public:
    ParallelServer();
    void open(int port);
    void stop();
    ~ParallelServer();
private:
    int server_fd;
    bool is_server_open;
    std::unique_ptr<std::thread> client_handling_loop;
    bool is_valid_solve_command(std::string line, std::vector<std::string>* solve_params);
    void transfer_client(int client_socket);
    void look_for_clients(struct sockaddr_in address);
};
