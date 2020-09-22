#include <string>

#include "../matrices/matrix_class.hpp"
#include "matrix_searcher.hpp"

class DFSMatrixSearcher : public MatrixSearcher {
public:
  // constructor
  DFSMatrixSearcher();

  // copy constructor
  DFSMatrixSearcher(const DFSMatrixSearcher &other);

  // copy assignment
  DFSMatrixSearcher &operator=(const DFSMatrixSearcher &other);

  /* problem contains a matrix and start / end coordinates.
     assigns a description of a path from start to end into solution
     and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              int *weight);

  // destructor
  ~DFSMatrixSearcher();

private:
  /* problem contains a matrix and start / end coordinates.
     returns true if there is a path in the matrix from start to end.
     if so, assigns a description of the path into solution
     and its weight into weight. */
  bool is_there_path(const Problem &problem, int i, int j,
                     std::unique_ptr<Matrix> &visited, std::string *solution,
                     int *weight);
};