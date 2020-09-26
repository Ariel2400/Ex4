#pragma once

#include <memory>
#include <string>

#include "problem.hpp"

typedef enum {
  PATH_FOUND = 0,
  PATH_NOT_FOUND = 1,
  MISSING_BREAK_LINE = 2,
  UNKNOWN_ALGORITHM = 3,
  WRONG_MATRIX_DIMENSIONS_FORMAT = 4,
  WRONG_NUMBER_OF_LINES = 5,
  WRONG_LINE_FORMAT = 6,
  WRONG_NUMBER_OF_CELLS_IN_LINE = 7,
  WRONG_START_CELL_FORMAT = 8,
  WRONG_END_CELL_FORMAT = 9,
} SearchStatus;

class MatrixSearcher {
public:
  /* problem contains a matrix and start / end coordinates.
     assigns a description of a path from start to end uint32_to solution
     and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              double *weight) = 0;
  virtual ~MatrixSearcher() = default;
};