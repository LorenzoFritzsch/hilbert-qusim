#ifndef LAZY_VECTOR_OPERATION_SCALAR_PRODUCT_H
#define LAZY_VECTOR_OPERATION_SCALAR_PRODUCT_H
#include "lazy_vector_operation.h"

class LazyVectorOperationScalarProduct : public LazyVectorOperation {
public:
  LazyVectorOperationScalarProduct(std::unique_ptr<ComplexVector> v, std::unique_ptr<Complex> k)
    : v_(std::move(v)),
      k_(std::move(k)) {
  }

  Complex get(int i) override;

  int size() override;

  [[nodiscard]] std::unique_ptr<LazyVectorOperation> clone() const override;

private:
  const std::unique_ptr<ComplexVector> v_;
  const std::unique_ptr<Complex> k_;
};

#endif // !LAZY_VECTOR_OPERATION_SCALAR_PRODUCT_H
