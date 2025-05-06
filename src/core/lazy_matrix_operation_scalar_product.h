#ifndef LAZY_MATRIX_OPERATION_SCALAR_PRODUCT_H
#define LAZY_MATRIX_OPERATION_SCALAR_PRODUCT_H
#include "lazy_matrix_operation.h"

class LazyMatrixOperationScalarProduct final : public LazyMatrixOperation {
public:
  LazyMatrixOperationScalarProduct(std::unique_ptr<ComplexMatrix> m, std::unique_ptr<Complex> k)
    : m_(std::move(m)),
      k_(std::move(k)) {
  }

  Complex get(int m, int n) override;

  int c_row_size() override;

  int c_column_size() override;

  [[nodiscard]] std::unique_ptr<LazyMatrixOperation> clone() const override;

private:
  const std::unique_ptr<ComplexMatrix> m_;
  const std::unique_ptr<Complex> k_;
};

#endif // !LAZY_MATRIX_OPERATION_SCALAR_PRODUCT_H
