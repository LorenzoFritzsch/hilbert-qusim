#include "lazy_matrix_operation_lazy_tensor_product.h"

Complex LazyMatrixOperationLazyTensorProduct::get(const int m, const int n) {
  const auto a_m = get_index(m, b_->row_size());
  const auto a_n = get_index(n, b_->column_size());
  const auto b_m = m % b_->row_size();
  const auto b_n = n % b_->column_size();
  return a_->get(a_m, a_n) * b_->get(b_m, b_n);
}

int LazyMatrixOperationLazyTensorProduct::c_row_size() {
  return a_->row_size() * b_->row_size();
}

int LazyMatrixOperationLazyTensorProduct::c_column_size() {
  return a_->column_size() * b_->column_size();
}

std::unique_ptr<LazyMatrixOperation> LazyMatrixOperationLazyTensorProduct::clone() const {
  return std::make_unique<LazyMatrixOperationLazyTensorProduct>(std::make_unique<LazyMatrix>(*a_),
                                                                std::make_unique<LazyMatrix>(*b_));
}
