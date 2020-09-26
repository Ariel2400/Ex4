#pragma once

#include <string>

#include "matrix_searcher.hpp"

struct Step {
public:
  uint32_t row;
  uint32_t column;
  std::string path;
  double weight;

  Step(uint32_t new_row, uint32_t new_column, std::string new_path, 
        double new_weight) {
    this->row = new_row;
    this->column = new_column;
    this->path = new_path;
    this->weight = new_weight;
  }
};

class BFSMatrixSearcher : public MatrixSearcher {
public:
  // constructor
  BFSMatrixSearcher() = default;

  // copy constructor
  BFSMatrixSearcher(const BFSMatrixSearcher &other) = default;

  // copy assignment
  BFSMatrixSearcher &operator=(const BFSMatrixSearcher &other) = default;

  /* problem contains a matrix and start / end coordinates.
     assigns a description of a path from start to end into solution
     and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              double *weight);

  // destructor
  ~BFSMatrixSearcher() = default;
};