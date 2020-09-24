#include <iostream>

#include "A-star_matrix_searcher.hpp"

#define BLOCK 0
#define CLOSED 1
#define NOT_CLOSED 0

AstarMatrixSearcher::AstarMatrixSearcher() {}

AstarMatrixSearcher::AstarMatrixSearcher(const AstarMatrixSearcher &other) {}

AstarMatrixSearcher &
AstarMatrixSearcher::operator=(const AstarMatrixSearcher &other) {
  return *this;
}

void AstarMatrixSearcher::update_in_queue(
    std::priority_queue<Step, std::vector<Step>, CompareWeight> *queue,
    Step newStep) {
  std::priority_queue<Step, std::vector<Step>, CompareWeight> temp;
  bool found = false;
  while (!queue->empty()) {
    Step step = queue->top();
    if (step.row == newStep.row && step.column == newStep.column) {
      found = true;
      if (step.f <= newStep.f) {
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

SearchStatus AstarMatrixSearcher::search(const Problem &problem,
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
  Step start(problem.start_row, problem.start_column, "",
             matrix->get_value(problem.start_row, problem.start_column));
  start.setHeuristic(problem.end_row, problem.end_column);
  open.push(start);
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
      *weight = step.f;
      return PATH_FOUND;
    }
    if (step.row - 1 >= 0) {
      Step successor(step.row - 1, step.column, step.path + "Up,",
                     step.g + matrix->get_value(step.row - 1, step.column));
      successor.setHeuristic(problem.end_row, problem.end_column);
      successors.push_back(successor);
    }
    if (step.row + 1 < height) {
      Step successor(step.row + 1, step.column, step.path + "Down,",
                     step.g + matrix->get_value(step.row + 1, step.column));
      successor.setHeuristic(problem.end_row, problem.end_column);
      successors.push_back(successor);
    }
    if (step.column - 1 >= 0) {
      Step successor(step.row, step.column - 1, step.path + "Left,",
                     step.g + matrix->get_value(step.row, step.column - 1));
      successor.setHeuristic(problem.end_row, problem.end_column);
      successors.push_back(successor);
    }
    if (step.column + 1 < width) {
      Step successor(step.row, step.column + 1, step.path + "Right,",
                     step.g + matrix->get_value(step.row, step.column + 1));
      successor.setHeuristic(problem.end_row, problem.end_column);
      successors.push_back(successor);
    }
    for (uint32_t i = 0; i < successors.size(); ++i) {
      if (matrix->get_value(successors.at(i).row, successors.at(i).column) !=
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

AstarMatrixSearcher::~AstarMatrixSearcher() {}

uint32_t main() {
  std::unique_ptr<AstarMatrixSearcher> searcher =
      std::make_unique<AstarMatrixSearcher>();
  Problem problem = {std::make_unique<Matrix>(3, 3), 0, 0, 0, 0};
  for (uint32_t i = 0; i < problem.matrix->get_height(); ++i) {
    for (uint32_t j = 0; j < problem.matrix->get_width(); ++j) {
      problem.matrix->set_value(i, j, 1);
    }
  }
  // problem.matrix->set_value(0, 0, 0);
  problem.matrix->set_value(1, 1, 0);
  problem.matrix->set_value(2, 0, 2);
  problem.matrix->set_value(1, 2, 6);
  // problem.matrix->set_value(3, 3, 0);
  // problem.matrix->set_value(3, 2, 0);
  std::string solution = "";
  uint32_t weight = 0;
  SearchStatus status = searcher->search(problem, &solution, &weight);
  std::cout << solution << std::endl;
  std::cout << weight << std::endl;
  std::cout << status << std::endl;
}
