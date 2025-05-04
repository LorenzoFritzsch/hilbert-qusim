#ifndef LAZY_VECTOR_OPERATION_LAZY_TENSOR_PRODUCT_H
#define LAZY_VECTOR_OPERATION_LAZY_TENSOR_PRODUCT_H
#include "lazy_vector.h"
#include "lazy_vector_operation.h"

class LazyVectorOperationLazyTensorProduct : public LazyVectorOperation {
public:
  LazyVectorOperationLazyTensorProduct(std::unique_ptr<LazyVector> v1, std::unique_ptr<LazyVector> v2)
    : v1_(std::move(v1)),
      v2_(std::move(v2)) {
  }

  Complex get(int i) override;

  int size() override;

  [[nodiscard]] std::unique_ptr<LazyVectorOperation> clone() const override;

private:
  const std::unique_ptr<LazyVector> v1_;
  const std::unique_ptr<LazyVector> v2_;
};

#endif // !LAZY_VECTOR_OPERATION_LAZY_TENSOR_PRODUCT_H
