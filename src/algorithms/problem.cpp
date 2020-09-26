#include "problem.hpp"

Problem::Problem(std::shared_ptr<Matrix> new_matrix, uint32_t new_start_row,
                 uint32_t new_start_column, uint32_t new_end_row, uint32_t new_end_column) {
  this->matrix = std::make_shared<Matrix>(*new_matrix);
  this->start_row = new_start_row;
  this->start_column = new_start_column;
  this->end_row = new_end_row;
  this->end_column = new_end_column;
}

Problem::Problem(Problem& other) {
  this->matrix = std::make_unique<Matrix>(*(other.matrix));
  this->start_row = other.start_row;
  this->start_column = other.start_column;
  this->end_row = other.end_row;
  this->end_column = other.end_column;
}