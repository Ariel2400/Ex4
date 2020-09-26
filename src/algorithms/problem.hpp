#pragma once

#include "matrix_class.hpp"
#include <memory>

class Problem {
public:
  std::shared_ptr<Matrix> matrix;
  uint32_t start_row;
  uint32_t start_column;
  uint32_t end_row;
  uint32_t end_column;

public:
  Problem(std::shared_ptr<Matrix> matrix, uint32_t start_row,
          uint32_t start_column, uint32_t end_row, uint32_t end_column);
  Problem(Problem& other);
  ~Problem() = default;
  Problem &operator=(Problem &other) = default;
};