#include "lazy_vector_operation_tensor_product.h"

Complex LazyVectorOperationTensorProduct::get(const int i) {
  const auto e1 = v1_->at(get_index(i, static_cast<int>(v1_->size())));
  const auto e2 = v2_->at(i % static_cast<int>(v2_->size()));
  return e1 * e2;
}

int LazyVectorOperationTensorProduct::size() {
  return static_cast<int>(v1_->size()) * static_cast<int>(v2_->size());
}

std::unique_ptr<LazyVectorOperation> LazyVectorOperationTensorProduct::clone() const {
  return std::make_unique<LazyVectorOperationTensorProduct>(std::make_unique<ComplexVector>(*v1_),
                                                            std::make_unique<ComplexVector>(*v2_));
}
