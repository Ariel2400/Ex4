#include "DFS_matrix_searcher.hpp"

#define BLOCK 0
#define VISITED 1
#define NOT_VISITED 0

// recursively looks for a path from each cell by looking for a path from the
// adjacent cells and marking visited cells
bool DFSMatrixSearcher::is_there_path(const Problem &problem, int i, int j,
                                      std::unique_ptr<Matrix> &visited,
                                      std::string *solution, double *weight) {
  std::unique_ptr<Matrix> matrix = std::make_unique<Matrix>(*(problem.matrix));
  uint32_t height = matrix->get_height();
  uint32_t width = matrix->get_width();
  if (i >= 0 && i < static_cast<int>(height) && j >= 0 && j < static_cast<int>(width) &&
      matrix->get_value(i, j) != BLOCK &&
      visited->get_value(i, j) == NOT_VISITED) {
    visited->set_value(i, j, VISITED);
    if (i == static_cast<int>(problem.end_row) && j == static_cast<int>(problem.end_column)) {
      *weight += matrix->get_value(i, j);
      return true;
    }
    if (is_there_path(problem, i - 1, j, visited, solution, weight)) {
      *solution = "Up," + *solution;
      *weight += matrix->get_value(i, j);
      return true;
    }
    if (is_there_path(problem, i + 1, j, visited, solution, weight)) {
      *solution = "Down," + *solution;
      *weight += matrix->get_value(i, j);
      return true;
    }
    if (is_there_path(problem, i, j - 1, visited, solution, weight)) {
      *solution = "Left," + *solution;
      *weight += matrix->get_value(i, j);
      return true;
    }
    if (is_there_path(problem, i, j + 1, visited, solution, weight)) {
      *solution = "Right," + *solution;
      *weight += matrix->get_value(i, j);
      return true;
    }
  }
  return false;
}

SearchStatus DFSMatrixSearcher::search(const Problem &problem,
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
  // mark which cells are done being developed
  std::unique_ptr<Matrix> visited = std::make_unique<Matrix>(height, width);
  for (uint32_t i = 0; i < height; ++i) {
    for (uint32_t j = 0; j < width; ++j) {
      visited->set_value(i, j, NOT_VISITED);
    }
  }
  *solution = "";
  *weight = 0;
  if (is_there_path(problem, problem.start_row, problem.start_column, visited,
                    solution, weight)) {
    (*solution).pop_back();
    return PATH_FOUND;
  }
  return PATH_NOT_FOUND;
}