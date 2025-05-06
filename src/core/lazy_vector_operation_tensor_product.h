#ifndef LAZY_VECTOR_OPERATION_TENSOR_PRODUCT_H
#define LAZY_VECTOR_OPERATION_TENSOR_PRODUCT_H
#include "lazy_vector_operation.h"

class LazyVectorOperationTensorProduct final : public LazyVectorOperation {
public:
  LazyVectorOperationTensorProduct(std::unique_ptr<ComplexVector> v1, std::unique_ptr<ComplexVector> v2)
    : v1_(std::move(v1)),
      v2_(std::move(v2)) {
  }

  Complex get(int i) override;

  int size() override;

  [[nodiscard]] std::unique_ptr<LazyVectorOperation> clone() const override;

private:
  const std::unique_ptr<ComplexVector> v1_;
  const std::unique_ptr<ComplexVector> v2_;
};

#endif // !LAZY_VECTOR_OPERATION_TENSOR_PRODUCT_H
