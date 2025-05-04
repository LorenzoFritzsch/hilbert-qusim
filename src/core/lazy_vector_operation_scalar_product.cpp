#include "lazy_vector_operation_scalar_product.h"

Complex LazyVectorOperationScalarProduct::get(const int i) {
  return v_->at(i) * *k_;
}

int LazyVectorOperationScalarProduct::size() {
  return static_cast<int>(v_->size());
}

std::unique_ptr<LazyVectorOperation> LazyVectorOperationScalarProduct::clone() const {
  return std::make_unique<LazyVectorOperationScalarProduct>(std::make_unique<ComplexVector>(*v_),
                                                            std::make_unique<Complex>(*k_));
}
