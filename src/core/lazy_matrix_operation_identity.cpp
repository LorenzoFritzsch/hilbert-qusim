#include "lazy_matrix_operation_identity.h"

Complex LazyMatrixOperationIdentity::get(const int m, const int n) {
  return m_->at(m).at(n);
}

int LazyMatrixOperationIdentity::c_row_size() {
  return static_cast<int>(m_->size());
}

int LazyMatrixOperationIdentity::c_column_size() {
  return static_cast<int>(m_->at(0).size());
}

std::unique_ptr<LazyMatrixOperation> LazyMatrixOperationIdentity::clone() const {
  return std::make_unique<LazyMatrixOperationIdentity>(std::make_unique<ComplexMatrix>(*m_));
}
