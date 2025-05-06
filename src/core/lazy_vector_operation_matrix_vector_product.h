#ifndef LAZY_VECTOR_OPERATION_MATRIX_VECTOR_PRODUCT_H
#define LAZY_VECTOR_OPERATION_MATRIX_VECTOR_PRODUCT_H
#include "lazy_vector_operation.h"

class LazyVectorOperationMatrixVectorProduct final : public LazyVectorOperation {
public:
  LazyVectorOperationMatrixVectorProduct(std::unique_ptr<ComplexMatrix> m, std::unique_ptr<ComplexVector> v)
    : m_(std::move(m)),
      v_(std::move(v)) {
  }

  Complex get(int i) override;

  int size() override;

  [[nodiscard]] std::unique_ptr<LazyVectorOperation> clone() const override;

private:
  const std::unique_ptr<ComplexMatrix> m_;
  const std::unique_ptr<ComplexVector> v_;
};

#endif // !LAZY_VECTOR_OPERATION_MATRIX_VECTOR_PRODUCT_H
