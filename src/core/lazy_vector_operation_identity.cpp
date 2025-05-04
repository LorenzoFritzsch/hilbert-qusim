#include "lazy_vector_operation_identity.h"

Complex LazyVectorOperationIdentity::get(const int i) {
  return v_->at(i);
}

int LazyVectorOperationIdentity::size() {
  return static_cast<int>(v_->size());
}

std::unique_ptr<LazyVectorOperation> LazyVectorOperationIdentity::clone() const {
  return std::make_unique<LazyVectorOperationIdentity>(std::make_unique<ComplexVector>(*v_));
}
