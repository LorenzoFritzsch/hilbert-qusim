#ifndef COMPLEX_VECTORISED_MATRIX_H
#define COMPLEX_VECTORISED_MATRIX_H

#include "complex_vector_split.h"
#include "hilbert_namespace.h"

class ComplexVectMatrix final {
public:
  ComplexVectMatrix(const ComplexMatrix &m)
      : row_size_(static_cast<int>(m.size())),
        column_size_(static_cast<int>(m.front().size())),
        vectorised_matrix_(ComplexVector(row_size_ * column_size_)) {
    const auto elements_size = row_size_ * column_size_;
    for (int i = 0; i < elements_size; i++) {
      vectorised_matrix_[i] = m.at(i / column_size_).at(i % column_size_);
    }
  }

  ComplexVectMatrix(ComplexVector m, const int row_size, const int column_size)
      : row_size_(row_size), column_size_(column_size),
        vectorised_matrix_(std::move(m)) {}

  ComplexVectMatrix(const ComplexVector &v)
      : row_size_(1), column_size_(static_cast<int>(v.size())),
        vectorised_matrix_(ComplexVector(row_size_ * column_size_)) {
    const auto elements_size = row_size_ * column_size_;
    for (int i = 0; i < elements_size; i++) {
      vectorised_matrix_[i] = v.at(i);
    }
  }

  ComplexVectMatrix(const Complex &c)
      : row_size_(1), column_size_(1), vectorised_matrix_(ComplexVector(1)) {
    vectorised_matrix_[0] = c;
  }

  ComplexVectMatrix()
      : row_size_(0), column_size_(0), vectorised_matrix_(ComplexVector(0)) {}

  [[nodiscard]] Complex get(const int m, const int n) const {
    return vectorised_matrix_.at(m * column_size_ + n);
  }

  [[nodiscard]] ComplexVectSplit get_row(const int row) const {
    auto start_index = row * column_size_;
    auto end_index = start_index + column_size_;
    return std::vector<Complex>(vectorised_matrix_.begin() + start_index,
                                vectorised_matrix_.begin() + end_index);
  }

  [[nodiscard]] ComplexVectSplit get_column(const int column) const {
    ComplexVectSplit result;
    for (int i = column; i < column_size_; i += column_size_) {
      result.add(vectorised_matrix_[i]);
    }
    return result;
  }

  [[nodiscard]] ComplexVectSplit split() const {
    ComplexVectSplit result;
    for (auto k : vectorised_matrix_) {
      result.add(k);
    }
    return result;
  }

  [[nodiscard]] int row_size() const { return row_size_; }

  [[nodiscard]] int column_size() const { return column_size_; }

private:
  const int row_size_;
  const int column_size_;
  ComplexVector vectorised_matrix_;
};

#endif // !COMPLEX_VECTORISED_MATRIX_H
