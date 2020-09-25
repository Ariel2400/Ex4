#include "server.hpp"
#include <string>
#include <vector>

class SerialServer: public Server
{
public:
    SerialServer();
    virtual void open(int port);
    virtual void stop();
    ~SerialServer();
private:
    int server_fd;
    bool is_server_open;
    bool is_client_active;
    std::unique_ptr<std::thread> client_handling_loop;
    void check_for_activity();
    bool is_valid_solve_command(std::string line, std::vector<std::string>* solve_params);
    void transfer_client(int client_socket);
    void look_for_clients(struct sockaddr_in address);
};
