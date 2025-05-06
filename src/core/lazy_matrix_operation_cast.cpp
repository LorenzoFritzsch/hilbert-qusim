#include "lazy_matrix_operation_cast.h"

Complex LazyMatrixOperationCast::get(const int m, const int n) {
  return m_->at(m).at(n);
}

int LazyMatrixOperationCast::c_row_size() {
  return static_cast<int>(m_->size());
}

int LazyMatrixOperationCast::c_column_size() {
  return static_cast<int>(m_->at(0).size());
}

std::unique_ptr<LazyMatrixOperation> LazyMatrixOperationCast::clone() const {
  return std::make_unique<LazyMatrixOperationCast>(std::make_unique<ComplexMatrix>(*m_));
}
