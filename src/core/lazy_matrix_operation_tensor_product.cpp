#include "lazy_matrix_operation_tensor_product.h"
#include <algorithm>

Complex LazyMatrixOperationTensorProduct::get(const int m, const int n) {
  const auto a_m = get_index(m, b_row_size());
  const auto a_n = get_index(n, b_column_size());
  const auto b_m = m % b_row_size();
  const auto b_n = n % b_column_size();
  return a_->at(a_m).at(a_n) * b_->at(b_m).at(b_n);
}

int LazyMatrixOperationTensorProduct::c_row_size() {
  return a_row_size() * b_row_size();
}

int LazyMatrixOperationTensorProduct::c_column_size() {
  return a_column_size() * b_column_size();
}

int LazyMatrixOperationTensorProduct::a_row_size() const {
  return static_cast<int>(a_->size());
}

int LazyMatrixOperationTensorProduct::a_column_size() const {
  return static_cast<int>(a_->at(0).size());
}

int LazyMatrixOperationTensorProduct::b_row_size() const {
  return static_cast<int>(b_->size());
}

int LazyMatrixOperationTensorProduct::b_column_size() const {
  return static_cast<int>(b_->at(0).size());
}

std::unique_ptr<LazyMatrixOperation> LazyMatrixOperationTensorProduct::clone() const {
  return std::make_unique<LazyMatrixOperationTensorProduct>(std::make_unique<ComplexMatrix>(*a_),
                                                            std::make_unique<ComplexMatrix>(*b_));
}
