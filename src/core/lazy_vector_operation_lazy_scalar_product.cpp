#include "lazy_vector_operation_lazy_scalar_product.h"

Complex LazyVectorOperationLazyScalarProduct::get(const int i) {
  return v_->get(i) * *k_;
}

int LazyVectorOperationLazyScalarProduct::size() {
  return v_->size();
}

std::unique_ptr<LazyVectorOperation> LazyVectorOperationLazyScalarProduct::clone() const {
  return std::make_unique<LazyVectorOperationLazyScalarProduct>(std::make_unique<LazyVector>(*v_),
                                                                std::make_unique<Complex>(*k_));
}
