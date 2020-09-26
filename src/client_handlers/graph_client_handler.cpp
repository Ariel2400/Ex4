#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <regex>
#include <unistd.h>
#include <sys/socket.h>
#include <algorithm>

#include "graph_client_handler.hpp"
GraphClientHandler::GraphClientHandler(MatrixSearcher *searcher){
  this->algorithm = searcher;
};

#define BUFFER_SIZE 4096
#define NONMEMBERLINES 4

GraphClientHandler::GraphClientHandler() {
  algorithms["A*"] = std::make_shared<AstarMatrixSearcher>();
  algorithms["bestfs"] = std::make_shared<BestFSMatrixSearcher>();
  algorithms["bfs"] = std::make_shared<BFSMatrixSearcher>();
  algorithms["dfs"] = std::make_shared<DFSMatrixSearcher>();
}


void GraphClientHandler::handle_client(int client_socket, std::string algorithm) {
  send_confirmation(client_socket);
  char buffer[BUFFER_SIZE] = {0}; 
  read(client_socket, buffer, BUFFER_SIZE);
  std::string problem_description(buffer);
  std::string two_line_breaks("\r\n\r\n");
  // the problem desription needs to end with two line breaks
  if (problem_description.length() < two_line_breaks.length() 
      || problem_description.substr(problem_description.length() - two_line_breaks.length(), two_line_breaks.length()).compare(two_line_breaks)) {
    send_response(client_socket, MISSING_BREAK_LINE, "", 0, "");
    return;
  }
  std::regex matrix_dimensions("([1-9][0-9]*),([1-9][0-9]*)");
  std::regex matrix_line("(([1-9][0-9]*(\\.[0-9]+)?|b|B),)*([1-9][0-9]*(\\.[0-9]+)?|b|B)");
  std::regex cell_indexes("([0-9]+),([0-9]+)");
  std::regex line_end_regex("(\r\n)+");
  // reading the matrix description line by line
  std::sregex_token_iterator line_iter(problem_description.begin(), problem_description.end(), line_end_regex, -1);
  std::sregex_token_iterator end;
  std::vector<std::string> lines;
  while(line_iter != end) {
    std::string line(*line_iter++);
    lines.push_back(line);
  }
  if (!std::regex_match(lines.at(0), matrix_dimensions)) {
    send_response(client_socket, WRONG_MATRIX_DIMENSIONS_FORMAT, "", 0, "");
    return;
  }
  uint32_t dimensions_comma_index = lines.at(0).find(',');
  uint32_t height = std::stoi(lines.at(0).substr(0, dimensions_comma_index));
  uint32_t width = std::stoi(lines.at(0).substr(dimensions_comma_index + 1, lines.at(0).length() - dimensions_comma_index - 1));
  if (lines.size() != height + 3) {
    send_response(client_socket, WRONG_NUMBER_OF_LINES, "", 0, "");
    return;
  }
  std::shared_ptr<Matrix> matrix = std::make_shared<Matrix>(height, width);
  for (uint32_t i = 0; i < height; ++i) {
    if (!std::regex_match(lines.at(i + 1), matrix_line)) {
      send_response(client_socket, WRONG_LINE_FORMAT, "", 0, "");
      return;
    }
    // parsing the values from each line by ","
    std::regex comma(",");
    std::sregex_token_iterator value_iter(lines.at(i + 1).begin(), lines.at(i + 1).end(), comma, -1);
    std::vector<std::string> line_values;
    while(value_iter != end) {
      std::string cell_value(*value_iter++);
      line_values.push_back(cell_value);
    }
    if (line_values.size() != width) {
      send_response(client_socket, WRONG_NUMBER_OF_CELLS_IN_LINE, "", 0, "");
      return;
    }
    for (uint32_t j = 0; j < width; ++j) {
      if (line_values.at(j).compare("b") == 0 || line_values.at(j).compare("B") == 0) {
        matrix->set_value(i, j, 0);
      } else {
        matrix->set_value(i, j, std::stod(line_values.at(j)));
      }
    }
    line_values.clear();
  }
  if (!std::regex_match(lines.at(height + 1), cell_indexes)) {
    send_response(client_socket, WRONG_START_CELL_FORMAT, "", 0, "");
    return;
  }
  uint32_t start_comma_index = lines.at(height + 1).find(',');
  uint32_t start_row = std::stoi(lines.at(height + 1).substr(0, start_comma_index));
  uint32_t start_column = std::stoi(lines.at(height + 1).substr(start_comma_index + 1, lines.at(0).length() - start_comma_index - 1));
  if (!std::regex_match(lines.at(height + 2), cell_indexes)) {
    send_response(client_socket, WRONG_END_CELL_FORMAT, "", 0, "");
    return;
  }
  uint32_t end_comma_index = lines.at(height + 2).find(',');
  uint32_t end_row = std::stoi(lines.at(height + 2).substr(0, end_comma_index));
  uint32_t end_column = std::stoi(lines.at(height + 2).substr(end_comma_index + 1, lines.at(0).length() - end_comma_index - 1));
  Problem problem(matrix, start_row, start_column, end_row, end_column);
  // searching the map for the fitting algorithm
  bool found_algorithm = false;
  for (auto& x: algorithms) {
    std::string algorithm_name_in_map(x.first);
    // converting the algorithm name to lowercase so that it's case-insensitive
    std::for_each(algorithm_name_in_map.begin(), algorithm_name_in_map.end(), [](char & c){
      c = ::tolower(c);
    });
    if (algorithm_name_in_map.compare(algorithm) == 0) {
      found_algorithm = true;
      std::string path("");
      double weight = 0;
      SearchStatus status = x.second->search(problem, &path, &weight);
      send_response(client_socket, status, path, weight, x.first);
      return;
    }
  }
  if (!found_algorithm) {
    send_response(client_socket, UNKNOWN_ALGORITHM, "", 0, "");
  }
}

void GraphClientHandler::send_confirmation(int client_socket) {
  std::string confirmation("Version: 1.0\r\nStatus: 0\r\nresponse-length: 0\r\n\r\n");
  send(client_socket , confirmation.c_str() , confirmation.length() , 0);
}

void GraphClientHandler::send_response(int client_socket, SearchStatus status, std::string path, double weight, std::string algorithm) {
  std::string response(std::to_string(weight) + "," + algorithm + "," + path);
  uint32_t response_length;
  if (status == PATH_FOUND) {
    response_length = response.length();
  } else {
    response_length = 0;
  }
  std::string message("Version: 1.0\r\nStatus: " + std::to_string(status) + "\r\nresponse-length: " + std::to_string(response_length) + "\r\n\r\n");
  if (status == PATH_FOUND) {
    message += response;
  }
  send(client_socket , message.c_str() , message.length() , 0);
}

GraphClientHandler::~GraphClientHandler() {
  algorithms.clear();
}

