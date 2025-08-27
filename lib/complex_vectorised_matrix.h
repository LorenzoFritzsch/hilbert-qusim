// Copyright 2025 Lorenzo Fritzsch
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COMPLEX_VECTORISED_MATRIX_H
#define COMPLEX_VECTORISED_MATRIX_H

#include "complex_vector_split.h"
#include "hilbert_namespace.h"
#include "simd.h"
#include <cstddef>
#include <functional>
#include <memory>

/*
 * This class represents a matrix in a vectorised form, in order to have all the
 * elements in a contiguous space and improve efficiency.
 */
class ComplexVectMatrix final {
public:
  explicit ComplexVectMatrix(const ComplexMatrix &m)
      : row_size_(m.size()), column_size_(m.front().size()),
        vectorised_matrix_(ComplexVector(row_size_ * column_size_)) {
    const auto elements_size = row_size_ * column_size_;
    for (size_t i = 0; i < elements_size; i++) {
      vectorised_matrix_[i] = m.at(i / column_size_).at(i % column_size_);
    }
  }

  ComplexVectMatrix(ComplexVector m, const size_t row_size,
                    const size_t column_size)
      : row_size_(row_size), column_size_(column_size),
        vectorised_matrix_(std::move(m)) {}

  explicit ComplexVectMatrix(const ComplexVector &v)
      : row_size_(1), column_size_(v.size()),
        vectorised_matrix_(ComplexVector(row_size_ * column_size_)) {
    const auto elements_size = row_size_ * column_size_;
    for (size_t i = 0; i < elements_size; i++) {
      vectorised_matrix_[i] = v.at(i);
    }
  }

  explicit ComplexVectMatrix(const Complex &c)
      : row_size_(1), column_size_(1), vectorised_matrix_(ComplexVector(1)) {
    vectorised_matrix_[0] = c;
  }

  ComplexVectMatrix()
      : row_size_(0), column_size_(0), vectorised_matrix_(ComplexVector(0)) {}

  bool operator==(const ComplexVectMatrix &other) const {
    if (row_size_ != other.row_size_ || column_size_ != other.column_size_) {
      return false;
    }

    auto diff = simd::cvsub(ComplexVectSplit(vectorised_matrix_),
                            ComplexVectSplit(other.vectorised_matrix_));
    if (!approx_equal(simd::cvsve(*diff), Complex(0, 0))) {
      return false;
    }
    return true;
  }

  [[nodiscard]] Complex get(const size_t m, const size_t n) const {
    return vectorised_matrix_.at(m * column_size_ + n);
  }

  [[nodiscard]] std::unique_ptr<ComplexVectSplit>
  get(size_t row_size, std::function<size_t(size_t i)> m_functor,
      std::function<size_t(size_t i)> n_functor) const {
    ComplexVectSplit result;
    for (size_t i = 0; i < row_size; i++) {
      auto m = m_functor(i);
      auto n = n_functor(i);
      auto index = m * column_size_ + n;
      result.add(vectorised_matrix_.at(index));
    }
    return std::make_unique<ComplexVectSplit>(result);
  }

  [[nodiscard]] std::unique_ptr<ComplexVectSplit>
  get_row(const size_t row) const {
    auto start_index = row * column_size_;
    auto end_index = start_index + column_size_;
    return std::make_unique<ComplexVectSplit>(
        std::vector<Complex>(vectorised_matrix_.begin() + start_index,
                             vectorised_matrix_.begin() + end_index));
  }

  [[nodiscard]] std::unique_ptr<ComplexVectSplit>
  get_column(const size_t column) const {
    ComplexVectSplit result;
    for (size_t m = 0; m < row_size_; m++) {
      result.add(vectorised_matrix_[m * column_size_ + column]);
    }
    return std::make_unique<ComplexVectSplit>(result);
  }

  [[nodiscard]] std::unique_ptr<ComplexVectSplit> split() const {
    ComplexVectSplit result;
    for (auto k : vectorised_matrix_) {
      result.add(k);
    }
    return std::make_unique<ComplexVectSplit>(result);
  }

  [[nodiscard]] size_t row_size() const { return row_size_; }

  [[nodiscard]] size_t column_size() const { return column_size_; }

  /*
   * Frequently used matrices and vectors.
   */
  static std::unique_ptr<ComplexVectMatrix> ket_0() {
    return std::make_unique<ComplexVectMatrix>(ComplexVector({1, 0}));
  }

  static std::unique_ptr<ComplexVectMatrix> ket_1() {
    return std::make_unique<ComplexVectMatrix>(ComplexVector({0, 1}));
  }

  static std::unique_ptr<ComplexVectMatrix> identity_2x2() {
    return std::make_unique<ComplexVectMatrix>(ComplexMatrix({{1, 0}, {0, 1}}));
  }

  static std::unique_ptr<ComplexVectMatrix> hadamard_2x2() {
    auto h = static_cast<__complex_precision>(1 / std::sqrt(2));
    return std::make_unique<ComplexVectMatrix>(ComplexMatrix({
        {h, h},
        {h, -h},
    }));
  }

  static std::unique_ptr<ComplexVectMatrix> pauli_x() {
    return std::make_unique<ComplexVectMatrix>(ComplexMatrix({{0, 1}, {1, 0}}));
  }

  static std::unique_ptr<ComplexVectMatrix> pauli_y() {
    return std::make_unique<ComplexVectMatrix>(
        ComplexMatrix({{0, Complex(0, -1)}, {Complex(0, 1), 0}}));
  }

  static std::unique_ptr<ComplexVectMatrix> pauli_z() {
    return std::make_unique<ComplexVectMatrix>(
        ComplexMatrix({{1, 0}, {0, -1}}));
  }

private:
  const size_t row_size_;
  const size_t column_size_;
  ComplexVector vectorised_matrix_;
};

#endif // !COMPLEX_VECTORISED_MATRIX_H
