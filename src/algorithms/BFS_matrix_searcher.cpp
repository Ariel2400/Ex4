#include <queue>
//#include <iostream>

#include "BFS_matrix_searcher.hpp"

#define BLOCK 0
#define VISITED 1
#define NOT_VISITED 0

BFSMatrixSearcher::BFSMatrixSearcher() {}

BFSMatrixSearcher::BFSMatrixSearcher(const BFSMatrixSearcher &other) {}

BFSMatrixSearcher &
BFSMatrixSearcher::operator=(const BFSMatrixSearcher &other) {
  return *this;
}

SearchStatus BFSMatrixSearcher::search(const Problem &problem,
                                       std::string *solution, int *weight) {
  std::unique_ptr<Matrix> matrix = std::make_unique<Matrix>(*(problem.matrix));
  int height = matrix->get_height();
  int width = matrix->get_width();
  // check if start and end coordinates are correct
  if (problem.start_row < 0 || problem.start_row >= height ||
      problem.start_column < 0 || problem.start_column >= width ||
      problem.end_row < 0 || problem.end_row >= height ||
      problem.end_column < 0 || problem.end_column >= width) {
    return OUT_OF_BOUNDS_INDEX;
  }
  // check if the path starts and ends on the same cell
  if (problem.start_row == problem.end_row &&
      problem.start_column == problem.end_column) {
    *solution = "";
    *weight = 0;
    return PATH_FOUND;
  }
  // for cells that are being developed
  std::queue<Step> step_queue;
  // mark which cells have already been visited
  std::unique_ptr<Matrix> visited = std::make_unique<Matrix>(height, width);
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (matrix->get_value(i, j) == BLOCK) {
        visited->set_value(i, j, VISITED);
      } else {
        visited->set_value(i, j, NOT_VISITED);
      }
    }
  }
  step_queue.push(
      Step(problem.start_row, problem.start_column, "",
           matrix->get_value(problem.start_row, problem.start_column)));
  visited->set_value(problem.start_row, problem.start_column, VISITED);
  while (!step_queue.empty()) {
    Step step = step_queue.front();
    step_queue.pop();
    // check if the algorithm reached the end
    if (step.row == problem.end_row && step.column == problem.end_column) {
      step.path.pop_back();
      *solution = step.path;
      *weight = step.weight;
      return PATH_FOUND;
    }
    // the adjacent cells
    if (step.row - 1 >= 0 &&
        visited->get_value(step.row - 1, step.column) == 0) {
      step_queue.push(
          Step(step.row - 1, step.column, step.path + "Up,",
               step.weight + matrix->get_value(step.row - 1, step.column)));
      visited->set_value(step.row - 1, step.column, VISITED);
    }
    if (step.row + 1 < height &&
        visited->get_value(step.row + 1, step.column) == 0) {
      step_queue.push(
          Step(step.row + 1, step.column, step.path + "Down,",
               step.weight + matrix->get_value(step.row + 1, step.column)));
      visited->set_value(step.row + 1, step.column, VISITED);
    }
    if (step.column - 1 >= 0 &&
        visited->get_value(step.row, step.column - 1) == 0) {
      step_queue.push(
          Step(step.row, step.column - 1, step.path + "Left,",
               step.weight + matrix->get_value(step.row, step.column - 1)));
      visited->set_value(step.row, step.column - 1, VISITED);
    }
    if (step.column + 1 < width &&
        visited->get_value(step.row, step.column + 1) == 0) {
      step_queue.push(
          Step(step.row, step.column + 1, step.path + "Right,",
               step.weight + matrix->get_value(step.row, step.column + 1)));
      visited->set_value(step.row, step.column + 1, VISITED);
    }
  }
  return PATH_NOT_FOUND;
}

BFSMatrixSearcher::~BFSMatrixSearcher() {}

/*
int main() {
    std::unique_ptr<BFSMatrixSearcher> searcher =
std::make_unique<BFSMatrixSearcher>(); Problem problem =
{std::make_unique<Matrix>(4, 4), 0, 0, 3, 0}; for (int i = 0; i <
problem.matrix->get_height(); ++i) { for (int j =0; j <
problem.matrix->get_width(); ++j) { problem.matrix->set_value(i, j, 2);
        }
    }
    problem.matrix->set_value(1, 0, 0);
    problem.matrix->set_value(1, 1, 0);
    problem.matrix->set_value(1, 2, 0);
    problem.matrix->set_value(1, 3, 0);
    problem.matrix->set_value(3, 3, 0);
    problem.matrix->set_value(3, 2, 0);
    std::string solution;
    SearchStatus status = searcher->search(problem, &solution);
    std::cout << solution << std::endl;
    std::cout << status << std::endl;
}
*/