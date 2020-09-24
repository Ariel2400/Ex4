#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <regex>
#include <iostream>
#include <memory>
#include <time.h>
#include "./src/algorithms/A-star_matrix_searcher.hpp"
#include "./src/algorithms/BestFS_matrix_searcher.hpp"
#include "./src/algorithms/BFS_matrix_searcher.hpp"
#include "./src/algorithms/DFS_matrix_searcher.hpp"
#include "./src/algorithms/problem.hpp"

int main() {
    srand(time(NULL));

    std::ofstream out("matrices.txt", std::fstream::app);
    if (!out)
    {
        out.close();
        std::cout << "problem" << std::endl;
    }
    auto bfs = new BFSMatrixSearcher();
    auto dfs = new DFSMatrixSearcher();
    auto bsfs = new BestFSMatrixSearcher();
    auto afs = new AstarMatrixSearcher();
    for (auto i = 0; i < 450; ++i)
    {
        std::string matrix = "";
        int height = (std::rand() % 50) + 5;
        int width = (std::rand() % 50) + 5;
        matrix += std::to_string(height);
        matrix += ",";
        matrix += std::to_string(width);
        matrix += "\r\n";
        for (int j = 0; j < height; ++j) {
            for (int k = 0; k < width; ++k) {
                matrix += std::to_string((std::rand() % 100) + 1);
                if (k == width - 1) {
                    continue;
                }
                matrix += ",";
            }
            matrix += "\r\n";
        }
        std::string runMatrix = matrix;
        matrix += "\r\n";
        out << matrix;

        runMatrix += "0,0\r\n";
        runMatrix += std::to_string(height - 1);
        runMatrix += ",";
        runMatrix += std::to_string(width - 1);
        runMatrix += "\r\n";
        runMatrix += "\r\n";
        auto problem = parse_string(runMatrix);
        bfs->search(*problem, &runMatrix, 0);
        dfs->search(*problem, &runMatrix, 0);
        bsfs->search(*problem, &runMatrix, 0);
        afs->search(*problem, &runMatrix, 0);
    }
    
    for (auto i = 0; i < 50; ++i)
    {
        std::string matrix = "";
        int height = (std::rand() % 50) + 50;
        int width = (std::rand() % 50) + 51;
        matrix += std::to_string(height);
        matrix += ",";
        matrix += std::to_string(width);
        matrix += "\r\n";
        for (int j = 0; j < height; ++j) {
            for (int k = 0; k < width; ++k) {
                matrix += std::to_string((std::rand() % 100) + 1);
                if (k == width - 1) {
                    continue;
                }
                matrix += ",";
            }
            matrix += "\r\n";
        }
        std::string runMatrix = matrix;
        matrix += "\r\n";
        out << matrix;

        runMatrix += "0,0\r\n";
        runMatrix += std::to_string(height - 1);
        runMatrix += ",";
        runMatrix += std::to_string(width - 1);
        runMatrix += "\r\n";
        runMatrix += "\r\n";
        auto problem = parse_string(runMatrix);
        bfs->search(*problem, &runMatrix, 0);
        dfs->search(*problem, &runMatrix, 0);
        bsfs->search(*problem, &runMatrix, 0);
        afs->search(*problem, &runMatrix, 0);
    }


 out.close();
}
std::unique_ptr<Problem> parse_string(std::string matrix){
std::vector<std::string> lines;
  std::istringstream sst(matrix);
  std::string line;
  while (std::getline(sst, line)) {
    lines.push_back(line);
  }
  return parse_input(lines);
}
std::unique_ptr<Problem> parse_input(std::vector<std::string> input) {
  std::regex matrix_param("([1-9][0-9]*),([1-9][0-9]*)");
  std::regex matrix_line_format(
      "(([0-9]+((.[0-9]+)?)*)|b|B,)*(([0-9]+((.[0-9]+)?))|b|B)");
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
        if (height != input.size() - 4) {
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
          if(values[j] == "b"||values[j] == "B"){
            problem->matrix->set_value(i, j, 0.0);
          }
          problem->matrix->set_value(i, j, stod(values[j]));
        }
      }
    }
  }
  return problem;
