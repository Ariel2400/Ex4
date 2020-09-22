#include <memory>
#include <string>

#include "../matrices/matrix_class.hpp"

typedef enum {
  PATH_FOUND = 0,
  PATH_NOT_FOUND = 1,
  OUT_OF_BOUNDS_INDEX = 2,
} SearchStatus;

typedef struct {
  std::unique_ptr<Matrix> matrix;
  int start_row;
  int start_column;
  int end_row;
  int end_column;
} Problem;

class MatrixSearcher {
public:
  /* problem contains a matrix and start / end coordinates.
     assigns a description of a path from start to end into solution
     and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              int *weight) = 0;
};