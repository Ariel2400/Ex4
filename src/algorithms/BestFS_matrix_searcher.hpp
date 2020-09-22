#include <queue>
#include <string>
#include <vector>

#include "../matrices/matrix_class.hpp"
#include "matrix_searcher.hpp"

struct Step {
public:
  int row;
  int column;
  std::string path;
  int weight;

  Step(int row, int column, std::string path, int weight) {
    this->row = row;
    this->column = column;
    this->path = path;
    this->weight = weight;
  }
};

struct CompareWeight {
  bool operator()(Step const &step1, Step const &step2) {
    return step1.weight > step2.weight;
  }
};

class BestFSMatrixSearcher : public MatrixSearcher {
public:
  // constructor
  BestFSMatrixSearcher();

  // copy constructor
  BestFSMatrixSearcher(const BestFSMatrixSearcher &other);

  // copy assignment
  BestFSMatrixSearcher &operator=(const BestFSMatrixSearcher &other);

  /* problem contains a matrix and start / end coordinates.
     assigns a description of the most efficient path from start to end into
     solution and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              int *weight);

  // destructor
  ~BestFSMatrixSearcher();

private:
  /* adds newStep to queue it no step in queue matches its coordinates.
     if a step in queue matches its coordinates and has a less efficient path
     its replaced by newStep. */
  void update_in_queue(
      std::priority_queue<Step, std::vector<Step>, CompareWeight> *queue,
      Step newStep);
};