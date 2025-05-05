#include "lazy_matrix_operation_lazy_outer_product.h"

Complex LazyMatrixOperationLazyOuterProduct::get(const int m, const int n) {
  return v1_->get(m) * v2_->get(n);
}

int LazyMatrixOperationLazyOuterProduct::c_row_size() {
  return v1_->size();
}

int LazyMatrixOperationLazyOuterProduct::c_column_size() {
  return v2_->size();
}

std::unique_ptr<LazyMatrixOperation> LazyMatrixOperationLazyOuterProduct::clone() const {
  return std::make_unique<LazyMatrixOperationLazyOuterProduct>(std::make_unique<LazyVector>(*v1_),
                                                               std::make_unique<LazyVector>(*v2_));
}
