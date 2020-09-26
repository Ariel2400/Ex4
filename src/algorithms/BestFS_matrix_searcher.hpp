#pragma once

#include <queue>
#include <string>
#include <vector>

#include "matrix_searcher.hpp"

struct BestFSStep {
public:
  uint32_t row;
  uint32_t column;
  std::string path;
  double weight;

  BestFSStep(uint32_t new_row, uint32_t new_column, std::string new_path, double new_weight) {
    this->row = new_row;
    this->column = new_column;
    this->path = new_path;
    this->weight = new_weight;
  }
};

struct CompareWeight {
  bool operator()(BestFSStep const &step1, BestFSStep const &step2) {
    return step1.weight > step2.weight;
  }
};

class BestFSMatrixSearcher : public MatrixSearcher {
public:
  // constructor
  BestFSMatrixSearcher() = default;

  // copy constructor
  BestFSMatrixSearcher(const BestFSMatrixSearcher &other) = default;

  // copy assignment
  BestFSMatrixSearcher &operator=(const BestFSMatrixSearcher &other) = default;

  /* problem contains a matrix and start / end coordinates.
     assigns a description of the most efficient path from start to end into
     solution and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              double *weight);

  // destructor
  ~BestFSMatrixSearcher() = default;

private:
  /* adds new_step to queue it no step in queue matches its coordinates.
     if a step in queue matches its coordinates and has a less efficient path
     its replaced by new_step. */
  void update_in_queue(
      std::priority_queue<BestFSStep, std::vector<BestFSStep>, CompareWeight> *queue,
      BestFSStep new_step);
};