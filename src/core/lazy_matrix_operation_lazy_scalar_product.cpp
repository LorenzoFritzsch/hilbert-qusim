#include "lazy_matrix_operation_lazy_scalar_product.h"

Complex LazyMatrixOperationLazyScalarProduct::get(const int m, const int n) {
  return m_->get(m, n) * *k_;
}

int LazyMatrixOperationLazyScalarProduct::c_row_size() {
  return m_->row_size();
}

int LazyMatrixOperationLazyScalarProduct::c_column_size() {
  return m_->column_size();
}

std::unique_ptr<LazyMatrixOperation> LazyMatrixOperationLazyScalarProduct::clone() const {
  return std::make_unique<LazyMatrixOperationLazyScalarProduct>(std::make_unique<LazyMatrix>(*m_),
                                                                std::make_unique<Complex>(*k_));
}
