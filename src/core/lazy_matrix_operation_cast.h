#ifndef LAZY_MATRIX_OPERATION_IDENTITY_H
#define LAZY_MATRIX_OPERATION_IDENTITY_H
#include "lazy_matrix_operation.h"

class LazyMatrixOperationCast final : public LazyMatrixOperation {
public:
  explicit LazyMatrixOperationCast(std::unique_ptr<ComplexMatrix> m)
    : m_(std::move(m)) {
  }

  Complex get(int m, int n) override;

  int c_row_size() override;

  int c_column_size() override;

  [[nodiscard]] std::unique_ptr<LazyMatrixOperation> clone() const override;

private:
  const std::unique_ptr<ComplexMatrix> m_;
};

#endif // !LAZY_MATRIX_OPERATION_IDENTITY_H
