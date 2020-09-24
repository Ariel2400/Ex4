#include <memory>
#include <string>
#pragma once
#include "problem.hpp"
#include "../matrices/matrix_class.hpp"
#include "DFS_matrix_searcher.hpp"
#include "BestFS_matrix_searcher.hpp"
#include "BFS_matrix_searcher.hpp"
#include "A-star_matrix_searcher.hpp"

typedef enum {
  PATH_FOUND = 0,
  PATH_NOT_FOUND = 1,
  OUT_OF_BOUNDS_INDEX = 2,
} SearchStatus;

class MatrixSearcher {
public:
  /* problem contains a matrix and start / end coordinates.
     assigns a description of a path from start to end uint32_to solution
     and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              uint32_t *weight) = 0;
};