#include <iostream>
#include <memory>

#include "matrix_class.hpp"

using namespace std;

Matrix::Matrix(int height, int width) {
  ErrorCode code = matrix_create(&matrix, height, width);
  if (!error_isSuccess(code)) {
    std::cerr << error_getErrorMessage(code) << std::endl;
  }
}

Matrix::Matrix(const Matrix &other) {
  ErrorCode code = matrix_copy(&matrix, other.matrix);
  if (!error_isSuccess(code)) {
    std::cerr << error_getErrorMessage(code) << std::endl;
  }
}

Matrix &Matrix::operator=(const Matrix &other) {
  if (this != &other) {
    matrix_destroy(matrix);
    ErrorCode code = matrix_copy(&matrix, other.matrix);
    if (!error_isSuccess(code)) {
      std::cerr << error_getErrorMessage(code) << std::endl;
    }
  }
  return *this;
}

PMatrix Matrix::get_matrix() const { return this->matrix; }

int Matrix::get_height() const {
  uint32_t height;
  ErrorCode code = matrix_getHeight(matrix, &height);
  if (!error_isSuccess(code)) {
    std::cerr << error_getErrorMessage(code) << std::endl;
  }
  return height;
}

int Matrix::get_width() const {
  uint32_t width;
  ErrorCode code = matrix_getWidth(matrix, &width);
  if (!error_isSuccess(code)) {
    std::cerr << error_getErrorMessage(code) << std::endl;
  }
  return width;
}

void Matrix::set_value(int i, int j, double value) {
  ErrorCode code = matrix_setValue(matrix, i, j, value);
  if (!error_isSuccess(code)) {
    std::cerr << error_getErrorMessage(code) << std::endl;
  }
}

double Matrix::get_value(int i, int j) const {
  double result;
  ErrorCode code = matrix_getValue(matrix, i, j, &result);
  if (!error_isSuccess(code)) {
    std::cerr << error_getErrorMessage(code) << std::endl;
  }
  return result;
}

bool Matrix::add_matrix(Matrix matToAdd) {
  ErrorCode code = matrix_add(&matrix, matrix, matToAdd.matrix);
  if (!error_isSuccess(code)) {
    std::cerr << error_getErrorMessage(code) << std::endl;
    return false;
  }
  return true;
}

bool Matrix::multiply_by_matrix(const Matrix matToMultiplyBy) {
  ErrorCode code =
      matrix_multiplyMatrices(&matrix, matrix, matToMultiplyBy.matrix);
  if (!error_isSuccess(code)) {
    std::cerr << error_getErrorMessage(code) << std::endl;
    return false;
  }
  return true;
}

Matrix::~Matrix() { matrix_destroy(matrix); }
