#include "../matrices/matrix_class.hpp"
#include <memory>

class Problem {
public:
  std::unique_ptr<Matrix> matrix;
  uint32_t start_row;
  uint32_t start_column;
  uint32_t end_row;
  uint32_t end_column;

public:
  Problem(std::unique_ptr<Matrix> matrix, uint32_t start_row,
          uint32_t start_column, uint32_t end_row, uint32_t end_column);
    Problem(Problem& other);
  ~Problem();
  Problem &operator=(Problem &other);
};