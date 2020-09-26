#include <regex>
#include <vector>

#include "client_redirector.hpp"

ClientRedirector::ClientRedirector() {
    problems["find-graph-path"] = std::make_shared<GraphClientHandler>();
}

void ClientRedirector::transfer_client(std::string message, int client_socket) {
    std::string request(message);
    // convert the request to lowercase so that it's case-insensitive
    std::for_each(request.begin(), request.end(), [](char & c){
        c = ::tolower(c);
    });
    // the request needs to end with two line breaks
    std::string two_line_breaks("\r\n\r\n");
    if (request.length() < two_line_breaks.length() 
        || request.substr(request.length() - two_line_breaks.length(), two_line_breaks.length()).compare(two_line_breaks)) {
        throw std::system_error(EDOM, std::generic_category(), "a line gap is required");
    }
    // parsing the request parameters
    std::regex whitespace("\\s+");
    std::sregex_token_iterator iter(request.begin(), request.end(), whitespace, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> params;
    while(iter != end) {
        std::string param(*iter++);
        params.push_back(param);
    }
    if (params.size() != 3) {
        throw std::system_error(EDOM, std::generic_category(), "invalid number of arguments");
    }
    if (params.at(0).compare("solve")) {
        throw std::system_error(EDOM, std::generic_category(), "unknown command");
    }
    // searching the map for the fitting client handler
    bool found_problem = false;
    for (auto& x: problems) {
        if (params.at(1).compare(x.first) == 0) {
            found_problem = true;
            x.second->handle_client(client_socket, params.at(2));
        }
    }
    if (!found_problem) {
        throw std::system_error(EDOM, std::generic_category(), "unknown problem");
    }
}

ClientRedirector::~ClientRedirector() {
    problems.clear();
}


