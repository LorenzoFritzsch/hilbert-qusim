#ifndef LAZY_VECTOR_OPERATION_IDENTITY_H
#define LAZY_VECTOR_OPERATION_IDENTITY_H
#include "lazy_vector_operation.h"

class LazyVectorOperationCast final : public LazyVectorOperation {
public:
  explicit LazyVectorOperationCast(std::unique_ptr<ComplexVector> v)
    : v_(std::move(v)) {
  }

  Complex get(int i) override;

  int size() override;

  [[nodiscard]] std::unique_ptr<LazyVectorOperation> clone() const override;

private:
  const std::unique_ptr<ComplexVector> v_;
};

#endif // !LAZY_VECTOR_OPERATION_IDENTITY_H
