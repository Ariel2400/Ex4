#include "A-star_matrix_searcher.hpp"

#define BLOCK 0
#define CLOSED 1
#define NOT_CLOSED 0

void AstarMatrixSearcher::update_in_queue(
    std::priority_queue<AstarStep, std::vector<AstarStep>, CompareF> *queue,
    AstarStep new_step) {
  std::priority_queue<AstarStep, std::vector<AstarStep>, CompareF> temp;
  bool found = false;
  while (!queue->empty()) {
    AstarStep step = queue->top();
    if (step.row == new_step.row && step.column == new_step.column) {
      found = true;
      if (step.f <= new_step.f) {
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

SearchStatus AstarMatrixSearcher::search(const Problem &problem,
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
  std::priority_queue<AstarStep, std::vector<AstarStep>, CompareF> open;
  AstarStep start(problem.start_row, problem.start_column, "",
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
  std::vector<AstarStep> successors;
  while (!open.empty()) {
    AstarStep step = open.top();
    open.pop();
    // check if the algorithm reached the end
    if (step.row == problem.end_row && step.column == problem.end_column) {
      step.path.pop_back();
      *solution = step.path;
      *weight = step.f;
      return PATH_FOUND;
    }
    if (step.row >= 1) {
      AstarStep successor(step.row - 1, step.column, step.path + "Up,",
                     step.g + matrix->get_value(step.row - 1, step.column));
      successor.setHeuristic(problem.end_row, problem.end_column);
      successors.push_back(successor);
    }
    if (step.row + 1 < height) {
      AstarStep successor(step.row + 1, step.column, step.path + "Down,",
                     step.g + matrix->get_value(step.row + 1, step.column));
      successor.setHeuristic(problem.end_row, problem.end_column);
      successors.push_back(successor);
    }
    if (step.column >= 1) {
      AstarStep successor(step.row, step.column - 1, step.path + "Left,",
                     step.g + matrix->get_value(step.row, step.column - 1));
      successor.setHeuristic(problem.end_row, problem.end_column);
      successors.push_back(successor);
    }
    if (step.column + 1 < width) {
      AstarStep successor(step.row, step.column + 1, step.path + "Right,",
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