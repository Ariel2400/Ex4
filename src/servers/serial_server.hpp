#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <regex>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>

#include "server.hpp"
#define BUFFERSIZE 4096
#define SOCKETERROR (-1)
#define SERVERBAcKLOG 1

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

class SerialServer: Server
{
public:
    SerialServer();
    ~SerialServer();
    void open(int port) override;
    void stop() override;
    void handle_connection(int client_socket);
    int check(int exp, const char *msg);
};
