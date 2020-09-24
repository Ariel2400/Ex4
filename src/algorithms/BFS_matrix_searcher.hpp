#include <string>

#include "../matrices/matrix_class.hpp"
#include "matrix_searcher.hpp"

struct Step {
public:
  uint32_t row;
  uint32_t column;
  std::string path;
  uint32_t weight;

  Step(uint32_t row, uint32_t column, std::string path, uint32_t weight) {
    this->row = row;
    this->column = column;
    this->path = path;
    this->weight = weight;
  }
};

class BFSMatrixSearcher : public MatrixSearcher {
public:
  // constructor
  BFSMatrixSearcher();

  // copy constructor
  BFSMatrixSearcher(const BFSMatrixSearcher &other);

  // copy assignment
  BFSMatrixSearcher &operator=(const BFSMatrixSearcher &other);

  /* problem contains a matrix and start / end coordinates.
     assigns a description of a path from start to end into solution
     and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              uint32_t *weight);

  // destructor
  ~BFSMatrixSearcher();
};