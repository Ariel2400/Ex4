#pragma once

#include "matrix.hpp"
class Matrix {

public:
  Matrix(int width, int height);
  Matrix(const Matrix &other);
  Matrix &operator=(const Matrix &other);
  int get_height() const;
  int get_width() const;
  void set_value(int i, int j, double value);
  double get_value(int i, int j) const;
  // returns true if operation was done successfully, false if not
  bool add_matrix(const Matrix matToAdd);
  // returns true if operation was done successfully, false if not
  bool multiply_by_matrix(const Matrix matToMultiplyBy);
  ~Matrix();

private:
  PMatrix matrix = nullptr;
  PMatrix get_matrix() const;
};