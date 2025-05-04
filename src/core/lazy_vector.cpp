#include "lazy_vector.h"

Complex LazyVector::get(const int i) const {
  if (c_->at(i) != std::nullopt) {
    return *c_->at(i);
  }
  Complex v = operation_->get(i);
  c_->at(i) = v;
  return v;
}

ComplexVector LazyVector::get() const {
  auto result = ComplexVector(c_->size(), 0);
  for (int i = 0; i < c_->size(); i++) {
    result[i] = get(i);
  }
  return result;
}

int LazyVector::size() const {
  return static_cast<int>(c_->size());
}
