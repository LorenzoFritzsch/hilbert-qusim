#ifndef LAZY_MATRIX_OPERATION_LAZY_SUM_H
#define LAZY_MATRIX_OPERATION_LAZY_SUM_H
#include "lazy_matrix.h"
#include "lazy_matrix_operation.h"

class LazyMatrixOperationLazySum final : public LazyMatrixOperation {
public:
  LazyMatrixOperationLazySum(std::unique_ptr<LazyMatrix> a, std::unique_ptr<LazyMatrix> b)
    : a_(std::move(a)),
      b_(std::move(b)) {
    if (a_->row_size() != b_->row_size() || a_->column_size() != b_->column_size()) {
      throw std::invalid_argument("Matrix sizes do not match");
    }
  }

  Complex get(int m, int n) override;

  int c_row_size() override;

  int c_column_size() override;

  [[nodiscard]] std::unique_ptr<LazyMatrixOperation> clone() const override;

private:
  const std::unique_ptr<LazyMatrix> a_;
  const std::unique_ptr<LazyMatrix> b_;
};

#endif //LAZY_MATRIX_OPERATION_LAZY_SUM_H
