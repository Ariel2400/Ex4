#include "graph_client_handler.hpp"

void GraphClientHandler::handleClient(int cs, int ss) {
  char* buffer = (char*)malloc(BUFFER*sizeof(char));
  read(ss, (void*)buffer, BUFFER);
  std::vector<std::string> lines;
  std::stringstream ss(buffer);
  std::string line;
  while(std::getline(ss, line, "\\r\\n")){
    lines.push_back(line);
  }
  parse_input(lines);
  
};
Problem parse_input(std::vector<std::string> input){
  std::regex matrix_param("([1-9][0-9]*),([1-9][0-9]*)");
  std::regex matrix_line_format(
      "(\\s*[0-9]+((.[0-9]+)?)\\s*,)*\\s*[0-9]+((.[0-9]+)?)\\s*");
  std::regex io_line("([1-9][0-9]*|0),([1-9][0-9]*|0)");
  uint32_t line_len=-1;
  uint32_t height, width;
  std::unique_ptr<Problem> problem;
  for(uint32_t i = 0; i<input.size(); ++i){
    if(i==0){
      if(!std::regex_match(input[i], matrix_param)){
        return NULL;
      }
      else{
        uint32_t index = input[i].find(',');
        height = stoi(input[i].substr(0, index-1));
        if(height != input.size()-3){
          return NULL;
        }
        width = stoi(input[i].substr(index+1, input[i].size()-index-1));
        problem = std::make_unique<Problem>;
        problem->matrix=std::make_unique<Matrix>(height, width);
      }
    }
    if(i==input.size()-1){
      if(!std::regex_match(input[i], io_line)){
        return NULL;
      }
      else{
          
      }
    }
  }
}