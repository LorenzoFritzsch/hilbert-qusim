#ifndef LAZY_MATRIX_OPERATION_TENSOR_PRODUCT_H
#define LAZY_MATRIX_OPERATION_TENSOR_PRODUCT_H
#include "lazy_matrix_operation.h"

class LazyMatrixOperationTensorProduct : public LazyMatrixOperation {
public:
  LazyMatrixOperationTensorProduct(std::unique_ptr<ComplexMatrix> a, std::unique_ptr<ComplexMatrix> b)
    : a_(std::move(a)),
      b_(std::move(b)) {
  }

  Complex get(int m, int n) override;

  int c_row_size() override;

  int c_column_size() override;

  [[nodiscard]] std::unique_ptr<LazyMatrixOperation> clone() const override;

private:
  const std::unique_ptr<ComplexMatrix> a_;
  const std::unique_ptr<ComplexMatrix> b_;

  [[nodiscard]] int a_row_size() const;

  [[nodiscard]] int a_column_size() const;

  [[nodiscard]] int b_row_size() const;

  [[nodiscard]] int b_column_size() const;
};

#endif // !LAZY_MATRIX_OPERATION_TENSOR_PRODUCT_H
