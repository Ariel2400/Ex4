#include <string>
#include <regex>

#include "serial_server.hpp"
#include "parallel_server.hpp"

int main(int argc, char ** argv) {
    if (argc != 3) {
        throw std::system_error(EDOM, std::generic_category(), "invalid number of arguments");
    }
    std::regex positive_number("[1-9][0-9]*");
    uint32_t port;
    if (std::regex_match(argv[1], positive_number)) {
        port = std::stoi(argv[1]);
    } else {
        throw std::system_error(EDOM, std::generic_category(), "invalid port number");
    }
    std::string server_type(argv[2]);
    std::for_each(server_type.begin(), server_type.end(), [](char & c){
        c = ::tolower(c);
    });
    if (server_type.compare("serial") == 0) {
        SerialServer server;
        server.open(port);
    } else if (server_type.compare("parallel") == 0) {
        ParallelServer server;
        server.open(port);
    } else {
        throw std::system_error(EDOM, std::generic_category(), "invalid server type");
    }
}