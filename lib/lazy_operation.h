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

#ifndef LAZY_OPERATION_H
#define LAZY_OPERATION_H

#include "complex_vector_split.h"
#include "complex_vectorised_matrix.h"
#include "operation.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

/*
 * This class represents the binary tree of the operations. To assure that a
 * `LazyOperation` can always be solved, the base node must be an operation of
 * type `MatrixMatrix`, for this reason the only available constructors take
 * `ComplexVectMatrix` as input.
 * Lazy operations can be merged together to have one single `LazyOperation`.
 *
 * The actual tree structure is held in the vector of operations. The last
 * `Operation` in the vector is assured to be the last operation, so by calling
 * its `get` methods we are able to retrieve the specified element/row.
 */
class LazyOperation final {
public:
  using op_op =
      std::function<Complex(const Operation &left, const Operation &right,
                            const size_t m, const size_t n)>;
  using op_mat = std::function<Complex(const Operation &left,
                                       const ComplexVectMatrix &right,
                                       const size_t m, const size_t n)>;
  using mat_op = std::function<Complex(const ComplexVectMatrix &left,
                                       const Operation &right, const size_t m,
                                       const size_t n)>;
  using mat_mat = std::function<Complex(const ComplexVectMatrix &left,
                                        const ComplexVectMatrix &right,
                                        const size_t m, const size_t n)>;

  using op_op_row = std::function<std::unique_ptr<ComplexVectSplit>(
      const Operation &left, const Operation &right, const size_t row)>;
  using op_mat_row = std::function<std::unique_ptr<ComplexVectSplit>(
      const Operation &left, const ComplexVectMatrix &right, const size_t row)>;
  using mat_op_row = std::function<std::unique_ptr<ComplexVectSplit>(
      const ComplexVectMatrix &left, const Operation &right, const size_t row)>;
  using mat_mat_row = std::function<std::unique_ptr<ComplexVectSplit>(
      const ComplexVectMatrix &left, const ComplexVectMatrix &right,
      const size_t row)>;

  // Underlaying `Operation` cannot be moved.
  LazyOperation(const LazyOperation &) = delete;
  LazyOperation &operator=(const LazyOperation &) = delete;

  LazyOperation(const ComplexVectMatrix &left, const ComplexVectMatrix &right,
                mat_mat op, mat_mat_row op_row, const size_t final_row_size,
                const size_t final_column_size) {
    mat_vect_.push_back(left);
    mat_vect_.push_back(right);
    op_vect_.emplace_back(0, 1, mat_vect_, op_vect_, std::move(op),
                          std::move(op_row), final_row_size, final_column_size);
  }

  explicit LazyOperation(const ComplexVectMatrix &mat) {
    mat_vect_.push_back(mat);
    op_vect_.emplace_back(
        0, 0, mat_vect_, op_vect_,
        [this](const ComplexVectMatrix &left, const ComplexVectMatrix &,
               const size_t m,
               const size_t n) { return mat_vect_[0].get(m, n); },
        [this](const ComplexVectMatrix &left, const ComplexVectMatrix &right,
               const size_t row) { return mat_vect_[0].get_row(row); },
        mat.row_size(), mat.column_size());
  }

  LazyOperation(const ComplexVectMatrix &left, const LazyOperation &right,
                op_op op, op_op_row op_row, const size_t final_row_size,
                const size_t final_column_size)
      : LazyOperation(left) {
    append(right, op, op_row, final_row_size, final_column_size);
  }

  LazyOperation(const ComplexVectMatrix &mat, mat_mat_row op_row) {
    mat_vect_.push_back(mat);
    op_vect_.emplace_back(
        0, 0, mat_vect_, op_vect_,
        [this](const ComplexVectMatrix &left, const ComplexVectMatrix &,
               const size_t m,
               const size_t n) { return mat_vect_[0].get(m, n); },
        op_row, mat.row_size(), mat.column_size());
  }

  void append(const ComplexVectMatrix &mat, op_mat op, op_mat_row op_row,
              const size_t final_row_size, const size_t final_column_size) {
    mat_vect_.push_back(mat);
    op_vect_.emplace_back(op_vect_.size() - 1, mat_vect_.size() - 1, mat_vect_,
                          op_vect_, std::move(op), std::move(op_row),
                          final_row_size, final_column_size);
  }

  void append(const LazyOperation &lazy_op, op_op op, op_op_row op_row,
              const size_t final_row_size, const size_t column_size);

  [[nodiscard]] Complex get(const size_t m, const size_t n) const {
    return op_vect_.back().get(m, n);
  }

  [[nodiscard]] std::unique_ptr<ComplexVectSplit> get(const size_t row) const {
    return op_vect_.back().get(row);
  }

  [[nodiscard]] size_t row_size() const { return op_vect_.back().row_size(); }

  [[nodiscard]] size_t column_size() const {
    return op_vect_.back().column_size();
  }

  [[nodiscard]] std::unique_ptr<ComplexVectMatrix> to_matrix() const;

  [[nodiscard]] std::vector<ComplexVectMatrix> mat_vect() { return mat_vect_; }

  [[nodiscard]] std::vector<Operation> op_vect() const { return op_vect_; }

  // *** Frequently used matrices and vectors. ***

  /*
   * Returns the lazy identity matrix of size `size`.
   */
  [[nodiscard]] static std::unique_ptr<LazyOperation>
  identity(const size_t size) {
    return std::unique_ptr<LazyOperation>(new LazyOperation(
        [](const ComplexVectMatrix &left, const ComplexVectMatrix &right,
           const size_t m, const size_t n) -> Complex {
          if (m == n) {
            return 1;
          } else {
            return 0;
          }
        },
        [size](const ComplexVectMatrix &left, const ComplexVectMatrix &right,
               const size_t row) -> std::unique_ptr<ComplexVectSplit> {
          auto res = std::make_unique<ComplexVectSplit>();
          for (int i = 0; i < size; i++) {
            if (i != row) {
              res->add(0);
            } else {
              res->add(1);
            }
          }
          return res;
        },
        size, size));
  }

private:
  std::vector<ComplexVectMatrix> mat_vect_;
  std::vector<Operation> op_vect_;

  LazyOperation(mat_mat op, mat_mat_row op_row, const size_t row_size,
                const size_t col_size) {
    mat_vect_.push_back(*ComplexVectMatrix::identity_2x2());
    op_vect_.emplace_back(0, 0, mat_vect_, op_vect_, std::move(op),
                          std::move(op_row), row_size, col_size);
  }
};
#endif // !LAZY_OPERATION_H
