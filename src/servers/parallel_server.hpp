#pragma once

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
    // constructor
    ParallelServer();

    // opens the server's port
    void open(int port);

    // closes the server
    void stop();

    // destructor
    ~ParallelServer();
private:
    int server_fd;
    bool is_server_open;
    // the thread that searches for clients while the main thread is ready to stop the server
    std::unique_ptr<std::thread> client_handling_loop;
    // the threads that handle the clients in parallel
    std::shared_ptr<std::thread> thread_pool[THREAD_POOL_SIZE];
    // the queue that holds that client sockets which are on hold to be handled
    std::queue<int> client_queue;
    std::mutex client_queue_mutex;

    /* reads the solve command from the client socket and redirects the client.
       if the client does send a message the function sets is_active to true */
    void read_from_client(int client_socket, bool* is_active);

    // permanently searches for new clients and adds them to the client queue
    void look_for_clients(struct sockaddr_in address);

    // permanently checks the client queue for clients to handle
    void attend_client_queue();

    // sleeps for 5 seconds and then sets is_checking to false
    void check_for_activity(bool* is_checking);
};
