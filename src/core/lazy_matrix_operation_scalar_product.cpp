#include "lazy_matrix_operation_scalar_product.h"

Complex LazyMatrixOperationScalarProduct::get(const int m, const int n) {
  return m_->at(m).at(n) * *k_;
}

int LazyMatrixOperationScalarProduct::c_row_size() {
  return static_cast<int>(m_->size());
}

int LazyMatrixOperationScalarProduct::c_column_size() {
  return static_cast<int>(m_->at(0).size());
}

std::unique_ptr<LazyMatrixOperation> LazyMatrixOperationScalarProduct::clone() const {
  return std::make_unique<LazyMatrixOperationScalarProduct>(std::make_unique<ComplexMatrix>(*m_),
                                                            std::make_unique<Complex>(*k_));
}
