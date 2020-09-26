#include "BestFS_matrix_searcher.hpp"

#define BLOCK 0
#define CLOSED 1
#define NOT_CLOSED 0

void BestFSMatrixSearcher::update_in_queue(
    std::priority_queue<BestFSStep, std::vector<BestFSStep>, CompareWeight> *queue,
    BestFSStep new_step) {
  std::priority_queue<BestFSStep, std::vector<BestFSStep>, CompareWeight> temp;
  bool found = false;
  while (!queue->empty()) {
    BestFSStep step = queue->top();
    if (step.row == new_step.row && step.column == new_step.column) {
      found = true;
      if (step.weight <= new_step.weight) {
        temp.push(step);
      } else {
        temp.push(new_step);
      }
    } else {
      temp.push(step);
    }
    queue->pop();
  }
  if (!found) {
    temp.push(new_step);
  }
  while (!temp.empty()) {
    queue->push(temp.top());
    temp.pop();
  }
}

SearchStatus BestFSMatrixSearcher::search(const Problem &problem,
                                          std::string *solution, double *weight) {
  std::unique_ptr<Matrix> matrix = std::make_unique<Matrix>(*(problem.matrix));
  uint32_t height = matrix->get_height();
  uint32_t width = matrix->get_width();
  // check if the path starts and ends on the same cell
  if (problem.start_row == problem.end_row &&
      problem.start_column == problem.end_column) {
    *solution = "";
    *weight = 0;
    return PATH_FOUND;
  }
  // for cells that are being developed
  std::priority_queue<BestFSStep, std::vector<BestFSStep>, CompareWeight> open;
  open.push(BestFSStep(problem.start_row, problem.start_column, "",
                 matrix->get_value(problem.start_row, problem.start_column)));
  // mark which cells are done being developed
  std::unique_ptr<Matrix> closed = std::make_unique<Matrix>(height, width);
  for (uint32_t i = 0; i < height; ++i) {
    for (uint32_t j = 0; j < width; ++j) {
      closed->set_value(i, j, NOT_CLOSED);
    }
  }
  // the adjacent cells
  std::vector<BestFSStep> successors;
  while (!open.empty()) {
    BestFSStep step = open.top();
    open.pop();
    // check if the algorithm reached the end
    if (step.row == problem.end_row && step.column == problem.end_column) {
      step.path.pop_back();
      *solution = step.path;
      *weight = step.weight;
      return PATH_FOUND;
    }
    if (step.row >= 1) {
      successors.push_back(
          BestFSStep(step.row - 1, step.column, step.path + "Up,",
               step.weight + matrix->get_value(step.row - 1, step.column)));
    }
    if (step.row + 1 < height) {
      successors.push_back(
          BestFSStep(step.row + 1, step.column, step.path + "Down,",
               step.weight + matrix->get_value(step.row + 1, step.column)));
    }
    if (step.column >= 1) {
      successors.push_back(
          BestFSStep(step.row, step.column - 1, step.path + "Left,",
               step.weight + matrix->get_value(step.row, step.column - 1)));
    }
    if (step.column + 1 < width) {
      successors.push_back(
          BestFSStep(step.row, step.column + 1, step.path + "Right,",
               step.weight + matrix->get_value(step.row, step.column + 1)));
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