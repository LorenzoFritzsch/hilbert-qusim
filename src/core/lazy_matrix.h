#ifndef LAZY_MATRIX_H
#define LAZY_MATRIX_H

#include <memory>
#include "lazy_matrix_operation.h"

class LazyMatrix {
public:
  explicit LazyMatrix(std::unique_ptr<LazyMatrixOperation> operation)
    : operation_(std::move(operation)) {
    const auto c_row_size = operation_->c_row_size();
    const auto c_column_size = operation_->c_column_size();
    c_ = std::make_unique<ComplexOptionalMatrix>(c_row_size, ComplexOptionalVector(c_column_size, std::nullopt));
  }

  LazyMatrix(const LazyMatrix &other)
    : operation_(other.operation_->clone()),
      c_(std::make_unique<ComplexOptionalMatrix>(*other.c_)) {
  }

  [[nodiscard]] Complex get(int m, int n) const;

  [[nodiscard]] ComplexVector get_row(int m) const;

  [[nodiscard]] ComplexMatrix get(bool complete = true) const;

  [[nodiscard]] int row_size() const;

  [[nodiscard]] int column_size() const;

private:
  const std::unique_ptr<LazyMatrixOperation> operation_;
  std::unique_ptr<ComplexOptionalMatrix> c_;
};

#endif // !LAZY_MATRIX_H
