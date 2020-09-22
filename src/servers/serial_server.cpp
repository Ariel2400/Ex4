#include "serial_server.hpp"




int check(int exp, const char *msg){
    if(exp == SOCKETERROR){
        perror(msg);
        exit(1);
    }
    return exp;
}

void handle_connection(int client_socket, ClientHandler c){
    c.handleClient();
}


int start(int port){

}

void SerialServer::open(int port){
    int server_socket, client_socket, addr_size;
    SA_IN server_addr, client_addr;

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Bind Failed!");
    check(listen(server_socket, SERVERBAcKLOG), "Listen Failed!");

    while(true){
        char* bufferC=(char*)malloc(BUFFERSIZE*sizeof(char));
        read(server_socket, (void*)bufferC, BUFFERSIZE);
        std::string buffer(bufferC);
        is_valid_solve_command(buffer);
        printf("Waiting for connections...\n");
        addr_size = sizeof(SA_IN);
        check(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size), "accept failed");
        printf("Connected!");
        
    }
    
}

bool is_valid_solve_command(std::string command){
    std::regex valid_command("solve\\s+(find-graph-path)\\s+(DFS|BFS|BestFS|A\\*)\\r\\n", std::regex_constants::icase);
    return std::regex_match(command, valid_command);
}
void SerialServer::stop(){

}
SerialServer::~SerialServer(){

}
SerialServer::SerialServer(){
    
}
int main(int argc, char **argv){
    SerialServer p;
    ClientHandler c;
    p.open(atoi(argv[1]), &c);
}
