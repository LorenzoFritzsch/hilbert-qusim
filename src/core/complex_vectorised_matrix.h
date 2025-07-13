#ifndef COMPLEX_VECTORISED_MATRIX_H
#define COMPLEX_VECTORISED_MATRIX_H

#include "hilbert_namespace.h"
#include "operation_member.h"
#include <memory>

class ComplexVectMatrix final : public OpMember {
public:
  explicit ComplexVectMatrix(const ComplexMatrix &m)
      : row_size_(static_cast<int>(m.size())),
        column_size_(static_cast<int>(m.front().size())),
        vectorised_matrix_(ComplexVector(row_size_ * column_size_)) {
    const auto elements_size = row_size_ * column_size_;
    for (int i = 0; i < elements_size; i++) {
      vectorised_matrix_[i] = m.at(i / column_size_).at(i % column_size_);
    }
  }

  ComplexVectMatrix(const ComplexVector &m, int row_size, int column_size)
      : row_size_(row_size), column_size_(column_size), vectorised_matrix_(m) {}

  explicit ComplexVectMatrix(const ComplexVector &v)
      : row_size_(1), column_size_(static_cast<int>(v.size())),
        vectorised_matrix_(ComplexVector(row_size_ * column_size_)) {
    const auto elements_size = row_size_ * column_size_;
    for (int i = 0; i < elements_size; i++) {
      vectorised_matrix_[i] = v.at(i);
    }
  }

  explicit ComplexVectMatrix(const Complex &c)
      : row_size_(1), column_size_(1), vectorised_matrix_(ComplexVector(1)) {
    vectorised_matrix_[0] = c;
  }

  ComplexVectMatrix()
      : row_size_(0), column_size_(0), vectorised_matrix_(ComplexVector(0)) {}

  [[nodiscard]] Complex get(const int m, const int n) const override {
    return vectorised_matrix_.at(m * column_size_ + n);
  }

  [[nodiscard]] int row_size() const override { return row_size_; }

  [[nodiscard]] int column_size() const override { return column_size_; }

  [[nodiscard]] std::unique_ptr<OpMember> clone() const override {
    return std::make_unique<ComplexVectMatrix>(vectorised_matrix_, row_size_,
                                               column_size_);
  }

private:
  const int row_size_;
  const int column_size_;
  ComplexVector vectorised_matrix_;
};

#endif // !COMPLEX_VECTORISED_MATRIX_H
