#include "lazy_vector_operation_lazy_matrix_vector_product.h"

Complex LazyVectorOperationLazyMatrixVectorProduct::get(const int i) {
  return row_column_multiplication(m_->get_row(i), v_->get());
}

int LazyVectorOperationLazyMatrixVectorProduct::size() {
  return v_->size();
}

std::unique_ptr<LazyVectorOperation> LazyVectorOperationLazyMatrixVectorProduct::clone() const {
  return std::make_unique<LazyVectorOperationLazyMatrixVectorProduct>(std::make_unique<LazyMatrix>(*m_),
                                                                      std::make_unique<LazyVector>(*v_));
}
