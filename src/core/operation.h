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

#ifndef OPERATION_H
#define OPERATION_H

#include "complex_vector_split.h"
#include "complex_vectorised_matrix.h"
#include <cstddef>
#include <functional>
#include <variant>

enum OperationType {
  OperationOperation,
  OperationMatrix,
  MatrixOperation,
  MatrixMatrix
};

class Operation final {
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

  using op_op_row = std::function<ComplexVectSplit(
      const Operation &left, const Operation &right, const size_t row)>;
  using op_mat_row = std::function<ComplexVectSplit(
      const Operation &left, const ComplexVectMatrix &right, const size_t row)>;
  using mat_op_row = std::function<ComplexVectSplit(
      const ComplexVectMatrix &left, const Operation &right, const size_t row)>;
  using mat_mat_row = std::function<ComplexVectSplit(
      const ComplexVectMatrix &left, const ComplexVectMatrix &right,
      const size_t row)>;

  // TODO: Review
  Operation(Operation &&) = default;
  Operation &operator=(Operation &&) = delete;
  Operation(const Operation &) = default;
  Operation &operator=(const Operation &) = delete;

  Operation(size_t left_index, size_t right_index,
            const std::vector<ComplexVectMatrix> &mat_vect,
            const std::vector<Operation> &op_vect, op_op op, op_op_row op_row,
            const size_t final_row_size, const size_t final_column_size)
      : left_index_(left_index), right_index_(right_index),
        op_type_(OperationOperation), mat_vect_(mat_vect), op_vect_(op_vect),
        op_functor_(std::move(op)), op_row_functor_(std::move(op_row)),
        row_size_(final_row_size), column_size_(final_column_size) {}

  Operation(size_t left_index, size_t right_index,
            const std::vector<ComplexVectMatrix> &mat_vect,
            const std::vector<Operation> &op_vect, op_mat op, op_mat_row op_row,
            const size_t final_row_size, const size_t final_column_size)
      : left_index_(left_index), right_index_(right_index),
        op_type_(OperationMatrix), mat_vect_(mat_vect), op_vect_(op_vect),
        op_functor_(std::move(op)), op_row_functor_(std::move(op_row)),
        row_size_(final_row_size), column_size_(final_column_size) {}

  Operation(size_t left_index, size_t right_index,
            const std::vector<ComplexVectMatrix> &mat_vect,
            const std::vector<Operation> &op_vect, mat_op op, mat_op_row op_row,
            const size_t final_row_size, const size_t final_column_size)
      : left_index_(left_index), right_index_(right_index),
        op_type_(MatrixOperation), mat_vect_(mat_vect), op_vect_(op_vect),
        op_functor_(std::move(op)), op_row_functor_(std::move(op_row)),
        row_size_(final_row_size), column_size_(final_column_size) {}

  Operation(size_t left_index, size_t right_index,
            const std::vector<ComplexVectMatrix> &mat_vect,
            const std::vector<Operation> &op_vect, mat_mat op,
            mat_mat_row op_row, const size_t final_row_size,
            const size_t final_column_size)
      : left_index_(left_index), right_index_(right_index),
        op_type_(MatrixMatrix), mat_vect_(mat_vect), op_vect_(op_vect),
        op_functor_(std::move(op)), op_row_functor_(std::move(op_row)),
        row_size_(final_row_size), column_size_(final_column_size) {}

  [[nodiscard]] Complex get(const size_t m, const size_t n) const {
    switch (op_type_) {
    case OperationOperation: {
      auto op = std::get<op_op>(op_functor_);
      return op(op_vect_[left_index_], op_vect_[right_index_], m, n);
    }
    case OperationMatrix: {
      auto op = std::get<op_mat>(op_functor_);
      return op(op_vect_[left_index_], mat_vect_[right_index_], m, n);
    }
    case MatrixOperation: {
      auto op = std::get<mat_op>(op_functor_);
      return op(mat_vect_[left_index_], op_vect_[right_index_], m, n);
    }
    case MatrixMatrix: {
      auto op = std::get<mat_mat>(op_functor_);
      return op(mat_vect_[left_index_], mat_vect_[right_index_], m, n);
    }
    }
    throw std::logic_error("Unexpected OperationType");
  }

  [[nodiscard]] ComplexVectSplit get(const size_t row) const {
    switch (op_type_) {
    case OperationOperation: {
      auto op = std::get<op_op_row>(op_row_functor_);
      return op(op_vect_[left_index_], op_vect_[right_index_], row);
    }
    case OperationMatrix: {
      auto op = std::get<op_mat_row>(op_row_functor_);
      return op(op_vect_[left_index_], mat_vect_[right_index_], row);
    }
    case MatrixOperation: {
      auto op = std::get<mat_op_row>(op_row_functor_);
      return op(mat_vect_[left_index_], op_vect_[right_index_], row);
    }
    case MatrixMatrix: {
      auto op = std::get<mat_mat_row>(op_row_functor_);
      return op(mat_vect_[left_index_], mat_vect_[right_index_], row);
    }
    }
    throw std::logic_error("Unexpected OperationType");
  }

  [[nodiscard]] size_t row_size() const { return row_size_; }

  [[nodiscard]] size_t column_size() const { return column_size_; }

  [[nodiscard]] OperationType op_type() const { return op_type_; }

  [[nodiscard]] std::variant<Operation, ComplexVectMatrix> left() const {
    switch (op_type_) {
    case OperationOperation:
    case OperationMatrix:
      return op_vect_[left_index_];
    case MatrixOperation:
    case MatrixMatrix:
      return mat_vect_[left_index_];
    }
    throw std::logic_error("Unexpected OperationType");
  }

  [[nodiscard]] size_t left_index() const { return left_index_; }

  [[nodiscard]] std::variant<Operation, ComplexVectMatrix> right() const {
    switch (op_type_) {
    case OperationOperation:
    case MatrixOperation:
      return op_vect_[right_index_];
    case OperationMatrix:
    case MatrixMatrix:
      return mat_vect_[right_index_];
    }
    throw std::logic_error("Unexpected OperationType");
  }

  [[nodiscard]] size_t right_index() const { return right_index_; }

  [[nodiscard]] std::variant<op_op, op_mat, mat_op, mat_mat> op_functor() {
    return op_functor_;
  }

  [[nodiscard]] std::variant<op_op_row, op_mat_row, mat_op_row, mat_mat_row>
  op_row_functor() {
    return op_row_functor_;
  }

private:
  const size_t left_index_;
  const size_t right_index_;
  const OperationType op_type_;

  const std::vector<ComplexVectMatrix> &mat_vect_;
  const std::vector<Operation> &op_vect_;

  std::variant<op_op, op_mat, mat_op, mat_mat> op_functor_;
  std::variant<op_op_row, op_mat_row, mat_op_row, mat_mat_row> op_row_functor_;

  size_t row_size_;
  size_t column_size_;
};

#endif // !OPERATION_H
