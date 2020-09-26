#pragma once

#include <string>

#include "matrix_searcher.hpp"

class DFSMatrixSearcher : public MatrixSearcher {
public:
  // constructor
  DFSMatrixSearcher() = default;

  // copy constructor
  DFSMatrixSearcher(const DFSMatrixSearcher &other) = default;

  // copy assignment
  DFSMatrixSearcher &operator=(const DFSMatrixSearcher &other) = default;

  /* problem contains a matrix and start / end coordinates.
     assigns a description of a path from start to end into solution
     and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              double *weight);

  // destructor
  ~DFSMatrixSearcher() = default;

private:
  /* problem contains a matrix and start / end coordinates.
     returns true if there is a path in the matrix from start to end.
     if so, assigns a description of the path into solution
     and its weight into weight. */
  bool is_there_path(const Problem &problem, uint32_t i, uint32_t j,
                     std::unique_ptr<Matrix> &visited, std::string *solution,
                     double *weight);
};