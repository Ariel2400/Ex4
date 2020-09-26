#pragma once

#include "server.hpp"

#include <string>
#include <vector>
#include <thread>

class SerialServer: public Server
{
public:
    // constructor
    SerialServer();

    // opens the server's port
    virtual void open(int port);

    // closes the server
    virtual void stop();

    // destructor
    ~SerialServer();
private:
    int server_fd;
    bool is_server_open;

    // the thread that searches for clients while the main thread is ready to stop the server
    std::unique_ptr<std::thread> client_handling_loop;

    // permanently searches for new clients
    void look_for_clients(struct sockaddr_in address);

    /* reads the solve command from the client socket and redirects the client.
       if the client does send a message the function sets is_active to true */
    void read_from_client(int client_socket, bool* is_active);

    // sleeps for 5 seconds and then sets is_checking to false
    void check_for_activity(bool* is_checking);
};
