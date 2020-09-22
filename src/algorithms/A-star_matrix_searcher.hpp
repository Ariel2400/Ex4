#include <cmath>
#include <queue>
#include <string>
#include <vector>

#include "../matrices/matrix_class.hpp"
#include "matrix_searcher.hpp"

#define MIN_STEP_WEIGHT 1

struct Step {
public:
  int row;
  int column;
  std::string path;
  int g;
  int h;
  int f;

  Step(int row, int column, std::string path, int g) {
    this->row = row;
    this->column = column;
    this->path = path;
    this->g = g;
  }

  void setHeuristic(int end_row, int end_column) {
    this->h = MIN_STEP_WEIGHT * (std::abs(end_row - this->row) +
                                 std::abs(end_column - this->column));
    this->f = this->g + this->h;
  }
};

struct CompareWeight {
  bool operator()(Step const &step1, Step const &step2) {
    return step1.f > step2.f;
  }
};

class AstarMatrixSearcher : public MatrixSearcher {
public:
  // constructor
  AstarMatrixSearcher();

  // copy constructor
  AstarMatrixSearcher(const AstarMatrixSearcher &other);

  // copy assignment
  AstarMatrixSearcher &operator=(const AstarMatrixSearcher &other);

  /* problem contains a matrix and start / end coordinates.
     assigns a description of the most efficient path from start to end into
     solution and its weight into weight. returns a fitting search status. */
  virtual SearchStatus search(const Problem &problem, std::string *solution,
                              int *weight);

  // destructor
  ~AstarMatrixSearcher();

private:
  /* adds newStep to queue it no step in queue matches its coordinates.
     if a step in queue matches its coordinates and has a less efficient path
     its replaced by newStep. */
  void update_in_queue(
      std::priority_queue<Step, std::vector<Step>, CompareWeight> *queue,
      Step newStep);
};