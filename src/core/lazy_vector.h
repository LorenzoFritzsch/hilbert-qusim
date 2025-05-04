#ifndef LAZY_VECTOR_H
#define LAZY_VECTOR_H

#include <memory>
#include "lazy_vector_operation.h"

class LazyVector {
public:
  explicit LazyVector(std::unique_ptr<LazyVectorOperation> operation)
    : operation_(std::move(operation)) {
    c_ = std::make_unique<ComplexOptionalVector>(operation_->size(), std::nullopt);
  }

  LazyVector(const LazyVector &other)
    : operation_(other.operation_->clone()),
      c_(std::make_unique<ComplexOptionalVector>(*other.c_)) {
  }

  [[nodiscard]] Complex get(int i) const;

  [[nodiscard]] ComplexVector get() const;

  [[nodiscard]] int size() const;

private:
  const std::unique_ptr<LazyVectorOperation> operation_;
  std::unique_ptr<ComplexOptionalVector> c_;
};

#endif // !LAZY_VECTOR_H
