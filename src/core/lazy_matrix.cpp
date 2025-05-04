#include "lazy_matrix.h"

ComplexMatrix to_complex_matrix(const ComplexOptionalMatrix& opt_matrix) {
  ComplexMatrix result;
  result.reserve(opt_matrix.size());

  for (const auto& row : opt_matrix) {
    ComplexVector new_row;
    new_row.reserve(row.size());
    for (const auto& elem : row) {
      new_row.push_back(elem.value_or(Complex(0)));
    }
    result.push_back(std::move(new_row));
  }
  return result;
}

Complex LazyMatrix::get(const int m, const int n) const {
  if ((*c_)[m][n] != std::nullopt) {
    return *(*c_)[m][n];
  }
  Complex v = operation_->get(m, n);
  (*c_)[m][n] = v;
  return v;
}

ComplexVector LazyMatrix::get_row(const int m) const {
  auto result = ComplexVector((*c_)[m].size(), 0);
  for (int i = 0; i < (*c_)[m].size(); i++) {
    result[i] = get(m, i);
  }
  return result;
}

ComplexMatrix LazyMatrix::get(const bool complete) const {
  if (!complete) {
    return to_complex_matrix(*c_);
  }
  auto result = ComplexMatrix(c_->size(), ComplexVector(c_->size(), 0));
  for (int m = 0; m < row_size(); m++) {
    for (int n = 0; n < column_size(); n++) {
      result[m][n] = get(m, n);
    }
  }
  return result;
}

int LazyMatrix::row_size() const {
  return static_cast<int>(c_->size());
}

int LazyMatrix::column_size() const {
  return static_cast<int>(c_->at(0).size());
}