#include "lazy_vector_operation_cast.h"

Complex LazyVectorOperationCast::get(const int i) {
  return v_->at(i);
}

int LazyVectorOperationCast::size() {
  return static_cast<int>(v_->size());
}

std::unique_ptr<LazyVectorOperation> LazyVectorOperationCast::clone() const {
  return std::make_unique<LazyVectorOperationCast>(std::make_unique<ComplexVector>(*v_));
}
