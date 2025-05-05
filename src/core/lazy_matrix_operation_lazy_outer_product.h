#ifndef LAZY_MATRIX_OPERATION_LAZY_OUTER_PRODUCT_H
#define LAZY_MATRIX_OPERATION_LAZY_OUTER_PRODUCT_H
#include "lazy_matrix_operation.h"
#include "lazy_vector.h"

class LazyMatrixOperationLazyOuterProduct final : public LazyMatrixOperation {
public:
  LazyMatrixOperationLazyOuterProduct(std::unique_ptr<LazyVector> v1, std::unique_ptr<LazyVector> v2)
    : v1_(std::move(v1)),
      v2_(std::move(v2)) {
  }

  Complex get(int m, int n) override;

  int c_row_size() override;

  int c_column_size() override;

  [[nodiscard]] std::unique_ptr<LazyMatrixOperation> clone() const override;

private:
  const std::unique_ptr<LazyVector> v1_;
  const std::unique_ptr<LazyVector> v2_;
};

#endif // !LAZY_MATRIX_OPERATION_LAZY_OUTER_PRODUCT_H
