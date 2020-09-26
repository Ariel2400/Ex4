#pragma once

#include <cmath>
#include <queue>
#include <string>
#include <vector>

#include "matrix_searcher.hpp"

#define MIN_STEP_WEIGHT 1

struct AstarStep {
public:
  uint32_t row;
  uint32_t column;
  std::string path;
  double g;
  double h;
  double f;

  AstarStep(uint32_t new_row, uint32_t new_column, std::string new_path, 
            double new_g) {
    this->row = new_row;
    this->column = new_column;
    this->path = new_path;
    this->g = new_g;
  }

  void setHeuristic(uint32_t end_row, uint32_t end_column) {
    this->h = MIN_STEP_WEIGHT * (std::abs(static_cast<int>(end_row) 
                                 - static_cast<int>(this->row)) 
                                 + std::abs(static_cast<int>(end_column) 
                                 - static_cast<int>(this->column)));
    this->f = this->g + this->h;
  }
};

struct CompareF {
  bool operator()(AstarStep const &step1, AstarStep const &step2) {
    return step1.f > step2.f;
  }
};

class AstarMatrixSearcher : public MatrixSearcher {
public:
  // constructor
  AstarMatrixSearcher() = default;

  // copy constructor
  AstarMatrixSearcher(const AstarMatrixSearcher &other) = default;

  // copy assignment
  AstarMatrixSearcher &operator=(const AstarMatrixSearcher &other) = default;

  /* problem contains a matrix and start / end coordinates.
     assigns a description of the most efficient path from start to end into
     solution and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              double *weight);

  // destructor
  ~AstarMatrixSearcher() = default;

private:
  /* adds new_step to queue it no step in queue matches its coordinates.
     if a step in queue matches its coordinates and has a less efficient path
     its replaced by new_step. */
  void update_in_queue(
      std::priority_queue<AstarStep, std::vector<AstarStep>, CompareF> *queue,
      AstarStep new_step);
};