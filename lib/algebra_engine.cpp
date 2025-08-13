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

#include "algebra_engine.h"

#include "complex_vector_split.h"
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include "operation.h"
#include "simd.h"
#include <complex>
#include <memory>

/*
 * Conjugate transpose
 */
Complex conjugate_transpose_lazy(const ComplexVectMatrix &left,
                                 const ComplexVectMatrix &right, const size_t m,
                                 const size_t n) {
  return std::conj(left.get(n, m));
}

ComplexVectSplit conjugate_transpose_row(const ComplexVectMatrix &left,
                                         const ComplexVectMatrix &right,
                                         const size_t row) {
  return left.get_column(row).conj();
}

/*
 * Inner product
 */
Complex inner_product_mat_mat(const ComplexVectMatrix &left,
                              const ComplexVectMatrix &right, const size_t m,
                              const size_t n) {

  auto left_split = left.split().conj();
  auto right_split = right.split();

  auto elements = simd::cvmul(left_split, right_split);

  __complex_precision result_real = 0;
  __complex_precision result_imag = 0;

  return simd::cvsve(elements);
}

ComplexVectSplit inner_product_mat_mat_row(const ComplexVectMatrix &left,
                                           const ComplexVectMatrix &right,
                                           const size_t row) {
  ComplexVectSplit result;
  result.add(inner_product_mat_mat(left, right, 0, 0));
  return result;
}

/*
 * Row-column multiplication
 */
Complex matrix_multiplication_op_op(const Operation &left,
                                    const Operation &right, const size_t row,
                                    const size_t col) {
  auto vect_left = left.get(row);
  ComplexVectSplit vect_right;
  for (size_t m = 0; m < right.row_size(); m++) {
    vect_right.add(right.get(m, col));
  }

  auto elements = simd::cvmul(vect_left, vect_right);

  return simd::cvsve(elements);
}

ComplexVectSplit matrix_multiplication_op_op_row(const Operation &left,
                                                 const Operation &right,
                                                 const size_t row) {
  ComplexVectSplit result;
  for (size_t n = 0; n < right.row_size(); n++) {
    result.add(matrix_multiplication_op_op(left, right, row, n));
  }
  return result;
}

Complex matrix_multiplication_mat_mat(const ComplexVectMatrix &left,
                                      const ComplexVectMatrix &right,
                                      const size_t row, const size_t col) {
  auto vect_left = left.get_row(row);
  ComplexVectSplit vect_right;
  for (size_t m = 0; m < right.row_size(); m++) {
    vect_right.add(right.get(m, col));
  }

  auto elements = simd::cvmul(vect_left, vect_right);

  return simd::cvsve(elements);
}

ComplexVectSplit
matrix_multiplication_mat_mat_row(const ComplexVectMatrix &left,
                                  const ComplexVectMatrix &right,
                                  const size_t row) {
  auto row_left = left.get_row(row);
  ComplexVectSplit result;
  for (size_t n = 0; n < right.column_size(); n++) {
    auto column_right = right.get_column(n);
    auto elements = simd::cvmul(row_left, column_right);
    result.add(simd::cvsve(elements));
  }
  return result;
}

Complex matrix_multiplication_op_mat(const Operation &left,
                                     const ComplexVectMatrix &right,
                                     const size_t row, const size_t col) {
  auto vect_left = left.get(row);
  auto vect_right = right.get_column(col);

  auto elements = simd::cvmul(vect_left, vect_right);

  return simd::cvsve(elements);
}

ComplexVectSplit matrix_multiplication_op_mat_row(
    const Operation &left, const ComplexVectMatrix &right, const size_t row) {
  auto row_left = left.get(row);
  ComplexVectSplit result;
  for (size_t n = 0; n < right.column_size(); n++) {
    auto column_right = right.get_column(n);
    auto elements = simd::cvmul(row_left, column_right);
    result.add(simd::cvsve(elements));
  }
  return result;
}

Complex matrix_vector_mul_mat_mat(const ComplexVectMatrix &left,
                                  const ComplexVectMatrix &right,
                                  const size_t row, const size_t col) {
  auto vect_left = left.get_row(col);
  auto vect_right = right.split();
  auto elements = simd::cvmul(vect_left, vect_right);
  return simd::cvsve(elements);
}

ComplexVectSplit matrix_vector_mul_mat_mat_row(const ComplexVectMatrix &left,
                                               const ComplexVectMatrix &right,
                                               const size_t row) {
  ComplexVectSplit result;
  for (size_t col = 0; col < right.column_size(); col++) {
    result.add(matrix_vector_mul_mat_mat(left, right, row, col));
  }
  return result;
}

Complex matrix_vector_mul_op_op(const Operation &left, const Operation &right,
                                const size_t row, const size_t col) {
  auto vect_left = left.get(col);
  ComplexVectSplit vect_right;
  for (size_t m = 0; m < right.column_size(); m++) {
    vect_right.add(right.get(0, m));
  }

  auto elements = simd::cvmul(vect_left, vect_right);

  return simd::cvsve(elements);
}

ComplexVectSplit matrix_vector_mul_op_op_row(const Operation &left,
                                             const Operation &right,
                                             const size_t row) {
  ComplexVectSplit result;
  for (size_t col = 0; col < right.column_size(); col++) {
    result.add(matrix_vector_mul_op_op(left, right, row, col));
  }
  return result;
}

size_t matrix_multiplication_final_row_size(size_t row_size_left,
                                            size_t column_size_left,
                                            size_t row_size_right,
                                            size_t column_size_right) {
  return row_size_left;
}

size_t matrix_multiplication_final_column_size(size_t row_size_left,
                                               size_t column_size_left,
                                               size_t row_size_right,
                                               size_t column_size_right) {
  return column_size_right;
}

/*
 * Outer product
 */
Complex outer_product_mat_mat(const ComplexVectMatrix &left,
                              const ComplexVectMatrix &right, const size_t m,
                              const size_t n) {
  return left.get(0, m) * std::conj(right.get(0, n));
}

ComplexVectSplit outer_product_mat_mat_row(const ComplexVectMatrix &left,
                                           const ComplexVectMatrix &right,
                                           const size_t row) {
  auto vect_left = left.get_row(0);
  auto vect_right = right.get_row(0).conj();

  return simd::cvmul(vect_left, vect_right);
}

/*
 * Scalar Product
 */
Complex scalar_product_mat_mat(const ComplexVectMatrix &left,
                               const ComplexVectMatrix &right, const size_t m,
                               const size_t n) {
  return left.get(m, n) * right.get(0, 0);
}

ComplexVectSplit scalar_product_mat_mat_row(const ComplexVectMatrix &left,
                                            const ComplexVectMatrix &right,
                                            const size_t row) {
  auto vect_left = left.get_row(row);
  auto k = right.get(0, 0);
  return simd::cvsmul(vect_left, k);
}

/*
 * Sum
 */
Complex sum_mat_mat(const ComplexVectMatrix &left,
                    const ComplexVectMatrix &right, const size_t m,
                    const size_t n) {
  return left.get(m, n) + right.get(m, n);
}

ComplexVectSplit sum_mat_mat_row(const ComplexVectMatrix &left,
                                 const ComplexVectMatrix &right,
                                 const size_t row) {
  ComplexVectSplit vect_left = left.get_row(row);
  ComplexVectSplit vect_right = right.get_row(row);

  return simd::cvadd(vect_left, vect_right);
}

Complex sum_op_op(const Operation &left, const Operation &right, const size_t m,
                  const size_t n) {
  return left.get(m, n) + right.get(m, n);
}

ComplexVectSplit sum_op_op_row(const Operation &left, const Operation &right,
                               const size_t row) {
  ComplexVectSplit vect_left = left.get(row);
  ComplexVectSplit vect_right = right.get(row);

  return simd::cvadd(vect_left, vect_right);
}

size_t sum_row_size(const size_t left_row_size, const size_t left_column_size,
                    const size_t right_row_size,
                    const size_t right_column_size) {
  return left_row_size;
}

size_t sum_column_size(const size_t left_row_size,
                       const size_t left_column_size,
                       const size_t right_row_size,
                       const size_t right_column_size) {
  return left_column_size;
}

/*
 * Tensor product
 */
Complex tensor_product_mat_mat(const ComplexVectMatrix &left,
                               const ComplexVectMatrix &right, const size_t m,
                               const size_t n) {
  const auto a_val = left.get(m / right.row_size(), n / right.column_size());
  const auto b_val = right.get(m % right.row_size(), n % right.column_size());
  return a_val * b_val;
}

Complex tensor_product_op_mat(const Operation &left,
                              const ComplexVectMatrix &right, const size_t m,
                              const size_t n) {
  const auto a_val = left.get(m / right.row_size(), n / right.column_size());
  const auto b_val = right.get(m % right.row_size(), n % right.column_size());
  return a_val * b_val;
}

/*
 * Tensor product left-hand elements, given the left-hand matrix of the
 * operation and the range.
 */
ComplexVectSplit tplhe(const ComplexVectMatrix &left, const size_t row,
                       const size_t row_size, const size_t right_row_size,
                       const size_t right_column_size) {
  return left.get(
      row_size,
      [row, right_row_size](size_t i) -> size_t {
        return row / right_row_size;
      },
      [right_column_size](size_t i) -> size_t {
        return i / right_column_size;
      });
}

ComplexVectSplit tplhe(const Operation &left, const size_t row,
                       const size_t row_size, const size_t right_row_size,
                       const size_t right_column_size) {
  ComplexVectSplit result;
  size_t m = row / right_row_size;
  auto left_row = left.get(m);
  for (size_t n = 0; n < row_size; n++) {
    result.add(left_row.get(n / right_column_size));
  }
  return result;
}

ComplexVectSplit tprhe(const ComplexVectMatrix &right, const size_t row,
                       const size_t row_size, const size_t right_row_size,
                       const size_t right_column_size) {
  return right.get(
      row_size,
      [row, right_row_size](size_t i) -> size_t {
        return row % right_row_size;
      },
      [right_column_size](size_t i) -> size_t {
        return i % right_column_size;
      });
}

ComplexVectSplit tensor_product_mat_mat_row(const ComplexVectMatrix &left,
                                            const ComplexVectMatrix &right,
                                            const size_t row) {
  ComplexVectSplit vect_left, vect_right;
  const auto right_row_size = right.row_size();
  const auto right_column_size = right.column_size();
  const auto final_row_size = left.row_size() * right_row_size;

  vect_left =
      tplhe(left, row, final_row_size, right_row_size, right_column_size);
  vect_right =
      tprhe(right, row, final_row_size, right_row_size, right_column_size);

  return simd::cvmul(vect_left, vect_right);
}

ComplexVectSplit tensor_product_op_mat_row(const Operation &left,
                                           const ComplexVectMatrix &right,
                                           const size_t row) {
  ComplexVectSplit vect_left, vect_right;
  const auto right_row_size = right.row_size();
  const auto right_column_size = right.column_size();
  const auto final_row_size = left.row_size() * right_row_size;

  vect_left =
      tplhe(left, row, final_row_size, right_row_size, right_column_size);
  vect_right =
      tprhe(right, row, final_row_size, right_row_size, right_column_size);

  return simd::cvmul(vect_left, vect_right);
}

size_t tensor_product_final_row_size(size_t row_size_left,
                                     size_t column_size_left,
                                     size_t row_size_right,
                                     size_t column_size_right) {
  return row_size_left * row_size_right;
}

size_t tensor_product_final_column_size(size_t row_size_left,
                                        size_t column_size_left,
                                        size_t row_size_right,
                                        size_t column_size_right) {
  return column_size_left * column_size_right;
}

std::unique_ptr<LazyOperation>
AlgebraEngine::conjugate_transpose(const ComplexVectMatrix &mat) {
  size_t final_row_size = mat.column_size();
  size_t final_column_size = mat.row_size();
  return std::make_unique<LazyOperation>(
      mat, ComplexVectMatrix(), conjugate_transpose_lazy,
      conjugate_transpose_row, final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::inner_product(const ComplexVectMatrix &vect_left,
                             const ComplexVectMatrix &vect_right) {
  if (vect_left.row_size() != 1 || vect_right.row_size() != 1) {
    throw std::invalid_argument("Input must be vectors");
  }
  return std::make_unique<LazyOperation>(vect_left, std::move(vect_right),
                                         inner_product_mat_mat,
                                         inner_product_mat_mat_row, 1, 1);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_multiplication(const ComplexVectMatrix &mat_left,
                                     const ComplexVectMatrix &mat_right) {
  size_t final_row_size = mat_left.row_size();
  size_t final_column_size = mat_right.column_size();
  return std::make_unique<LazyOperation>(
      mat_left, mat_right, matrix_multiplication_mat_mat,
      matrix_multiplication_mat_mat_row, final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(const ComplexVectMatrix &mat,
                                     const ComplexVectMatrix &vect) {
  size_t final_column_size = vect.column_size();
  return std::make_unique<LazyOperation>(mat, vect, matrix_vector_mul_mat_mat,
                                         matrix_vector_mul_mat_mat_row, 1,
                                         final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(std::unique_ptr<LazyOperation> mat,
                                     const ComplexVectMatrix &vect) {
  auto op = std::move(mat);
  op->append(vect, matrix_multiplication_op_mat,
             matrix_multiplication_op_mat_row,
             matrix_multiplication_final_row_size,
             matrix_multiplication_final_column_size);

  return std::move(op);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(std::unique_ptr<LazyOperation> mat,
                                     std::unique_ptr<LazyOperation> vect) {
  auto op = std::move(mat);
  op->append(*std::move(vect), matrix_vector_mul_op_op,
             matrix_vector_mul_op_op_row, matrix_multiplication_final_row_size,
             matrix_multiplication_final_column_size);

  return std::move(op);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::outer_product(const ComplexVectMatrix &mat_left,
                             const ComplexVectMatrix &mat_right) {
  size_t final_row_size = mat_left.column_size();
  size_t final_column_size = mat_right.column_size();
  return std::make_unique<LazyOperation>(
      mat_left, mat_right, outer_product_mat_mat, outer_product_mat_mat_row,
      final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::scalar_product(const ComplexVectMatrix &mat, const Complex &k) {
  size_t final_row_size = mat.row_size();
  size_t final_column_size = mat.column_size();
  return std::make_unique<LazyOperation>(
      mat, ComplexVectMatrix(k), scalar_product_mat_mat,
      scalar_product_mat_mat_row, final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::sum(const ComplexVectMatrix &mat_left,
                   const ComplexVectMatrix &mat_right) {
  size_t mat_left_row_size = mat_left.row_size();
  size_t mat_right_column_size = mat_right.column_size();
  if (mat_left_row_size != mat_right.row_size() ||
      mat_left.column_size() != mat_right_column_size) {
    throw std::invalid_argument("Matrix sizes do not match");
  }
  return std::make_unique<LazyOperation>(mat_left, mat_right, sum_mat_mat,
                                         sum_mat_mat_row, mat_left_row_size,
                                         mat_right_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::sum(const ComplexVectMatrix &mat_left,
                   const LazyOperation &mat_right) {
  size_t mat_left_row_size = mat_left.row_size();
  size_t mat_right_column_size = mat_right.column_size();
  if (mat_left_row_size != mat_right.row_size() ||
      mat_left.column_size() != mat_right_column_size) {
    throw std::invalid_argument("Matrix sizes do not match");
  }
  auto lazy = std::make_unique<LazyOperation>(mat_left);
  lazy->append(mat_right, sum_op_op, sum_op_op_row, sum_row_size,
               sum_column_size);
  return std::move(lazy);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const ComplexVectMatrix &mat_left,
                              const ComplexVectMatrix &mat_right) {
  size_t final_row_size = mat_left.row_size() * mat_right.row_size();
  size_t final_column_size = mat_left.column_size() * mat_right.column_size();
  return std::make_unique<LazyOperation>(
      mat_left, mat_right, tensor_product_mat_mat, tensor_product_mat_mat_row,
      final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const ComplexVectMatrix &mat,
                              const size_t times) {
  if (times == 1) {
    return std::make_unique<LazyOperation>(mat);
  }

  size_t final_row_size = mat.row_size() * mat.row_size();
  size_t final_column_size = mat.column_size() * mat.column_size();

  auto lazy = std::make_unique<LazyOperation>(
      mat, mat, tensor_product_mat_mat, tensor_product_mat_mat_row,
      final_row_size, final_column_size);

  for (size_t _ = 2; _ < times; _++) {
    lazy->append(mat, tensor_product_op_mat, tensor_product_op_mat_row,
                 tensor_product_final_row_size,
                 tensor_product_final_column_size);
  }
  return lazy;
}

bool AlgebraEngine::is_unitary(const ComplexVectMatrix &mat) {
  if (mat.row_size() != mat.column_size()) {
    return false;
  }

  auto a_dagger = AlgebraEngine::conjugate_transpose(mat);
  auto lazy = std::make_unique<LazyOperation>(mat);
  lazy->append(*a_dagger, matrix_multiplication_op_op,
               matrix_multiplication_op_op_row,
               matrix_multiplication_final_row_size,
               matrix_multiplication_final_column_size);

  for (size_t m = 0; m < lazy->row_size(); m++) {
    if (!approx_equal(lazy->get(m, m), Complex(1, 0))) {
      return false;
    }
    auto row = lazy->get(m);
    if (!approx_equal(simd::cvsve(row), Complex(1, 0))) {
      return false;
    }
  }
  return true;
}
