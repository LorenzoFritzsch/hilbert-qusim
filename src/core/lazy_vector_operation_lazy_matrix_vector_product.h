#ifndef LAZY_VECTOR_OPERATION_LAZY_MATRIX_VECTOR_PRODUCT_H
#define LAZY_VECTOR_OPERATION_LAZY_MATRIX_VECTOR_PRODUCT_H
#include "lazy_matrix.h"
#include "lazy_vector.h"
#include "lazy_vector_operation.h"

class LazyVectorOperationLazyMatrixVectorProduct final : public LazyVectorOperation {
public:
  LazyVectorOperationLazyMatrixVectorProduct(std::unique_ptr<LazyMatrix> m, std::unique_ptr<LazyVector> v)
    : m_(std::move(m)),
      v_(std::move(v)) {
  }

  Complex get(int i) override;

  int size() override;

  [[nodiscard]] std::unique_ptr<LazyVectorOperation> clone() const override;

private:
  const std::unique_ptr<LazyMatrix> m_;
  const std::unique_ptr<LazyVector> v_;
};
#endif // !LAZY_VECTOR_OPERATION_LAZY_MATRIX_VECTOR_PRODUCT_H
