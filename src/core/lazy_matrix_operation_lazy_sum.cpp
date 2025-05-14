#include "lazy_matrix_operation_lazy_sum.h"

Complex LazyMatrixOperationLazySum::get(const int m, const int n) {
  return a_->get(m, n) + b_->get(m, n);
}

int LazyMatrixOperationLazySum::c_row_size() {
  return a_->row_size();
}

int LazyMatrixOperationLazySum::c_column_size() {
  return a_->column_size();
}

std::unique_ptr<LazyMatrixOperation> LazyMatrixOperationLazySum::clone() const {
  return std::make_unique<LazyMatrixOperationLazySum>(std::make_unique<LazyMatrix>(*a_),
                                                      std::make_unique<LazyMatrix>(*b_));
}
