#include "lazy_vector_operation_matrix_vector_product.h"

Complex LazyVectorOperationMatrixVectorProduct::get(const int i) {
  return row_column_multiplication(m_->at(i), *v_);
}

int LazyVectorOperationMatrixVectorProduct::size() {
  return static_cast<int>(v_->size());
}

std::unique_ptr<LazyVectorOperation> LazyVectorOperationMatrixVectorProduct::clone() const {
  return std::make_unique<LazyVectorOperationMatrixVectorProduct>(std::make_unique<ComplexMatrix>(*m_),
                                                                  std::make_unique<ComplexVector>(*v_));
}
