#include "graph_client_handler.hpp"
GraphClientHandler::GraphClientHandler(MatrixSearcher *searcher){
  this->algorithm = searcher;
};

void GraphClientHandler::handleClient(int cs, int ss) {
  char *buffer = (char *)malloc(BUFFER * sizeof(char));
  read(ss, (void *)buffer, BUFFER);
  std::vector<std::string> lines;
  std::stringstream sst(buffer);
  std::string line;
  while (std::getline(sst, line)) {
    lines.push_back(line);
  }
  auto problem = parse_input(lines);

  std::string path;
  uint32_t *weight = 0;
  SearchStatus status = this->algorithm->search(*problem, &path, weight);
  std::string result("Version: 1.0\r\n");
  if(status == PATH_FOUND){
    result.append("status: " + std::to_string(status) + "\r\nresponse_length: " +std::to_string(sizeof(path))+"\r\n\r\n");
    result.append(path+"\r\n\r\n");
  }
  else{
    result.append("status: " + std::to_string(status));
  }
  write(cs)
};


std::unique_ptr<Problem> parse_input(std::vector<std::string> input) {
  std::regex matrix_param("([1-9][0-9]*),([1-9][0-9]*)");
  std::regex matrix_line_format(
      "(([0-9]+((.[0-9]+)?)*)|b,)*(([0-9]+((.[0-9]+)?))|b)");
  std::regex io_line("([1-9][0-9]*|0),([1-9][0-9]*|0)");
  uint32_t line_len = -1;
  uint32_t height, width;
  std::unique_ptr<Problem> problem;

  for (uint32_t i = 0; i < input.size(); ++i) {

    if (i == 0) {
      if (!std::regex_match(input[i], matrix_param)) {
        return NULL;
      }

      else {
        uint32_t index = input[i].find(',');
        height = stoi(input[i].substr(0, index - 1));
        if (height != input.size() - NONMEMBERLINES) {
          return NULL;
        }
        width = stoi(input[i].substr(index + 1, input[i].size() - index - 1));
        problem = std::make_unique<Problem>();
        problem->matrix = std::make_unique<Matrix>(height, width);
      }
    }

    else if (i == input.size() - 2) {
      if (!std::regex_match(input[i], io_line)) {
        return NULL;
      }

      else {
        uint32_t index = input[i].find(',');
        problem->start_row = stoi(input[i].substr(0, index - 1));
        problem->start_column =
            stoi(input[i].substr(index + 1, input[i].size() - index - 1));
      }
    }

    else if (i == input.size() - 1) {
      if (!std::regex_match(input[i], io_line)) {
        return NULL;
      }

      else {
        uint32_t index = input[i].find(',');
        problem->end_row = stoi(input[i].substr(0, index - 1));
        problem->end_column =
            stoi(input[i].substr(index + 1, input[i].size() - index - 1));
      }
    }

    else {
      if (!std::regex_match(input[i], matrix_line_format)) {
        return NULL;
      }

      else {
        if (input[i].size() != width) {
          return NULL;
        }
        std::vector<std::string> values;
        std::stringstream sst(input[i]);
        std::string value;
        while (std::getline(sst, value, ',')) {
          values.push_back(value);
        }
        for (int j = 0; j < values.size(); ++j) {
          if(values[j] == "b"){
            problem->matrix->set_value(i, j, 0.0);
          }
          problem->matrix->set_value(i, j, stod(values[j]));
        }
      }
    }
  }
  return problem;

  int main(){

  }
}