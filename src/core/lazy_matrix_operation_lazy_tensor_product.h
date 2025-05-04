#ifndef LAZY_MATRIX_OPERATION_LAZY_TENSOR_PRODUCT_H
#define LAZY_MATRIX_OPERATION_LAZY_TENSOR_PRODUCT_H
#include "lazy_matrix.h"
#include "lazy_matrix_operation.h"

class LazyMatrixOperationLazyTensorProduct : public LazyMatrixOperation {
public:
  LazyMatrixOperationLazyTensorProduct(std::unique_ptr<LazyMatrix> a, std::unique_ptr<LazyMatrix> b)
    : a_(std::move(a)),
      b_(std::move(b)) {
  }

  Complex get(int m, int n) override;

  int c_row_size() override;

  int c_column_size() override;

  [[nodiscard]] std::unique_ptr<LazyMatrixOperation> clone() const override;

private:
  const std::unique_ptr<LazyMatrix> a_;
  const std::unique_ptr<LazyMatrix> b_;
};

#endif //LAZY_MATRIX_OPERATION_LAZY_TENSOR_PRODUCT_H