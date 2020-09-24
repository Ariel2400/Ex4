#include <iostream>

#include "BestFS_matrix_searcher.hpp"

#define BLOCK 0
#define CLOSED 1
#define NOT_CLOSED 0

BestFSMatrixSearcher::BestFSMatrixSearcher() {}

BestFSMatrixSearcher::BestFSMatrixSearcher(const BestFSMatrixSearcher &other) {}

BestFSMatrixSearcher &
BestFSMatrixSearcher::operator=(const BestFSMatrixSearcher &other) {
  return *this;
}

void BestFSMatrixSearcher::update_in_queue(
    std::priority_queue<Step, std::vector<Step>, CompareWeight> *queue,
    Step newStep) {
  std::priority_queue<Step, std::vector<Step>, CompareWeight> temp;
  bool found = false;
  while (!queue->empty()) {
    Step step = queue->top();
    if (step.row == newStep.row && step.column == newStep.column) {
      found = true;
      if (step.weight <= newStep.weight) {
        temp.push(step);
      } else {
        temp.push(newStep);
      }
    } else {
      temp.push(step);
    }
    queue->pop();
  }
  if (!found) {
    temp.push(newStep);
  }
  while (!temp.empty()) {
    queue->push(temp.top());
    temp.pop();
  }
}

SearchStatus BestFSMatrixSearcher::search(const Problem &problem,
                                          std::string *solution, uint32_t *weight) {
  std::unique_ptr<Matrix> matrix = std::make_unique<Matrix>(*(problem.matrix));
  uint32_t height = matrix->get_height();
  uint32_t width = matrix->get_width();
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
  std::priority_queue<Step, std::vector<Step>, CompareWeight> open;
  open.push(Step(problem.start_row, problem.start_column, "",
                 matrix->get_value(problem.start_row, problem.start_column)));
  // mark which cells are done being developed
  std::unique_ptr<Matrix> closed = std::make_unique<Matrix>(height, width);
  for (uint32_t i = 0; i < height; ++i) {
    for (uint32_t j = 0; j < width; ++j) {
      closed->set_value(i, j, NOT_CLOSED);
    }
  }
  // the adjacent cells
  std::vector<Step> successors;
  while (!open.empty()) {
    Step step = open.top();
    open.pop();
    // check if the algorithm reached the end
    if (step.row == problem.end_row && step.column == problem.end_column) {
      step.path.pop_back();
      *solution = step.path;
      *weight = step.weight;
      return PATH_FOUND;
    }
    if (step.row - 1 >= 0) {
      successors.push_back(
          Step(step.row - 1, step.column, step.path + "Up,",
               step.weight + matrix->get_value(step.row - 1, step.column)));
    }
    if (step.row + 1 < height) {
      successors.push_back(
          Step(step.row + 1, step.column, step.path + "Down,",
               step.weight + matrix->get_value(step.row + 1, step.column)));
    }
    if (step.column - 1 >= 0) {
      successors.push_back(
          Step(step.row, step.column - 1, step.path + "Left,",
               step.weight + matrix->get_value(step.row, step.column - 1)));
    }
    if (step.column + 1 < width) {
      successors.push_back(
          Step(step.row, step.column + 1, step.path + "Right,",
               step.weight + matrix->get_value(step.row, step.column + 1)));
    }
    for (uint32_t i = 0; i < successors.size(); ++i) {
      if (successors.at(i).row >= 0 && successors.at(i).row < height &&
          successors.at(i).column >= 0 && successors.at(i).column < width &&
          matrix->get_value(successors.at(i).row, successors.at(i).column) !=
              BLOCK &&
          closed->get_value(successors.at(i).row, successors.at(i).column) !=
              CLOSED) {
        update_in_queue(&open, successors.at(i));
      }
    }
    successors.clear();
    // mark cell as developed
    closed->set_value(step.row, step.column, CLOSED);
  }
  return PATH_NOT_FOUND;
}

BestFSMatrixSearcher::~BestFSMatrixSearcher() {}

/*
int main() {
    std::unique_ptr<BestFSMatrixSearcher> searcher =
std::make_unique<BestFSMatrixSearcher>(); Problem problem =
{std::make_unique<Matrix>(3, 3), 0, 0, 2, 0}; for (int i = 0; i <
problem.matrix->get_height(); ++i) { for (int j =0; j <
problem.matrix->get_width(); ++j) { problem.matrix->set_value(i, j, 1);
        }
    }
    problem.matrix->set_value(1, 0, 0);
    problem.matrix->set_value(1, 1, 0);
    problem.matrix->set_value(1, 2, 0);
    //problem.matrix->set_value(1, 3, 0);
    //problem.matrix->set_value(3, 3, 0);
    //problem.matrix->set_value(3, 2, 0);
    std::string solution = "";
    int weight = 0;
    SearchStatus status = searcher->search(problem, &solution, &weight);
    std::cout << solution << std::endl;
    std::cout << weight << std::endl;
    std::cout << status << std::endl;
}
*/
