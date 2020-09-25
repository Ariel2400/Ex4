#include "problem.hpp"

Problem::Problem(std::unique_ptr<Matrix> matrix, uint32_t start_row,
                 uint32_t start_column, uint32_t end_row, uint32_t end_column) {
  this->matrix = std::make_unique<Matrix>(matrix);
  this->start_row = start_row;
  this->start_column = start_column;
  this->end_row = end_row;
  this->end_column = end_column;
};
Problem::Problem(Problem& other) {
  this->matrix = std::make_unique<Matrix>(other.matrix);
  this->start_row = other.start_row;
  this->start_column = other.start_column;
  this->end_row = other.end_row;
  this->end_column = other.end_column;
};
Problem::~Problem(){

};

Problem & Problem::operator=(Problem& other){
    return *this=Problem(other);
}