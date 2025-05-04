#include "lazy_vector_operation_lazy_tensor_product.h"

Complex LazyVectorOperationLazyTensorProduct::get(const int i) {
  const auto e1 = v1_->get(get_index(i, v1_->size()));
  const auto e2 = v2_->get(i % v2_->size());
  return e1 * e2;
}

int LazyVectorOperationLazyTensorProduct::size() {
  return v1_->size() * v2_->size();
}

std::unique_ptr<LazyVectorOperation> LazyVectorOperationLazyTensorProduct::clone() const {
  return std::make_unique<LazyVectorOperationLazyTensorProduct>(std::make_unique<LazyVector>(*v1_),
                                                                std::make_unique<LazyVector>(*v2_));
}
