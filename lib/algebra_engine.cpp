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
#include <stdexcept>

/*
 * Conjugate transpose
 */
inline Complex conjugate_transpose_lazy(const ComplexVectMatrix &left,
                                        const ComplexVectMatrix &right,
                                        const size_t m, const size_t n) {
  return std::conj(left.get(n, m));
}

inline std::unique_ptr<ComplexVectSplit>
conjugate_transpose_row(const ComplexVectMatrix &left,
                        const ComplexVectMatrix &right, const size_t row) {
  return left.get_column(row)->conj();
}

/*
 * Inner product
 */
inline Complex inner_product_mat_mat(const ComplexVectMatrix &left,
                                     const ComplexVectMatrix &right,
                                     const size_t m, const size_t n) {
  return simd::cvsve(*simd::cvmul(*left.split()->conj(), *right.split()));
}

inline std::unique_ptr<ComplexVectSplit>
inner_product_mat_mat_row(const ComplexVectMatrix &left,
                          const ComplexVectMatrix &right, const size_t row) {
  auto result = std::make_unique<ComplexVectSplit>();
  result->add(inner_product_mat_mat(left, right, 0, 0));
  return result;
}

/*
 * Matrix multiplication
 */
inline Complex matrix_multiplication_op_op(const Operation &left,
                                           const Operation &right,
                                           const size_t row, const size_t col) {
  ComplexVectSplit vect_right;
  for (size_t m = 0; m < right.row_size(); m++) {
    vect_right.add(right.get(m, col));
  }
  return simd::cvsve(*simd::cvmul(*left.get(row), vect_right));
}

inline std::unique_ptr<ComplexVectSplit>
matrix_multiplication_op_op_row(const Operation &left, const Operation &right,
                                const size_t row) {
  auto result = std::make_unique<ComplexVectSplit>();
  for (size_t n = 0; n < right.row_size(); n++) {
    result->add(matrix_multiplication_op_op(left, right, row, n));
  }
  return result;
}

inline Complex matrix_multiplication_mat_mat(const ComplexVectMatrix &left,
                                             const ComplexVectMatrix &right,
                                             const size_t row,
                                             const size_t col) {
  ComplexVectSplit vect_right;
  for (size_t m = 0; m < right.row_size(); m++) {
    vect_right.add(right.get(m, col));
  }
  return simd::cvsve(*simd::cvmul(*left.get_row(row), vect_right));
}

inline std::unique_ptr<ComplexVectSplit>
matrix_multiplication_mat_mat_row(const ComplexVectMatrix &left,
                                  const ComplexVectMatrix &right,
                                  const size_t row) {
  const auto row_left = left.get_row(row);
  auto result = std::make_unique<ComplexVectSplit>();
  for (size_t n = 0; n < right.column_size(); n++) {
    result->add(simd::cvsve(*simd::cvmul(*row_left, *right.get_column(n))));
  }
  return result;
}

inline Complex matrix_multiplication_op_mat(const Operation &left,
                                            const ComplexVectMatrix &right,
                                            const size_t row,
                                            const size_t col) {
  return simd::cvsve(*simd::cvmul(*left.get(row), *right.get_column(col)));
}

inline std::unique_ptr<ComplexVectSplit> matrix_multiplication_op_mat_row(
    const Operation &left, const ComplexVectMatrix &right, const size_t row) {
  const auto row_left = left.get(row);
  auto result = std::make_unique<ComplexVectSplit>();
  for (size_t n = 0; n < right.column_size(); n++) {
    result->add(simd::cvsve(*simd::cvmul(*row_left, *right.get_column(n))));
  }
  return result;
}

/*
 * Matrix-vector product
 */

inline Complex matrix_vector_mul_mat_mat(const ComplexVectMatrix &left,
                                         const ComplexVectMatrix &right,
                                         const size_t row, const size_t col) {
  return simd::cvsve(*simd::cvmul(*left.get_row(col), *right.split()));
}

inline std::unique_ptr<ComplexVectSplit>
matrix_vector_mul_mat_mat_row(const ComplexVectMatrix &left,
                              const ComplexVectMatrix &right,
                              const size_t row) {
  auto result = std::make_unique<ComplexVectSplit>();
  for (size_t col = 0; col < right.column_size(); col++) {
    result->add(matrix_vector_mul_mat_mat(left, right, row, col));
  }
  return result;
}

inline Complex matrix_vector_mul_op_op(const Operation &left,
                                       const Operation &right, const size_t row,
                                       const size_t col) {
  ComplexVectSplit vect_right;
  for (size_t m = 0; m < right.column_size(); m++) {
    vect_right.add(right.get(0, m));
  }
  return simd::cvsve(*simd::cvmul(*left.get(col), vect_right));
}

inline std::unique_ptr<ComplexVectSplit>
matrix_vector_mul_op_op_row(const Operation &left, const Operation &right,
                            const size_t row) {
  auto result = std::make_unique<ComplexVectSplit>();
  for (size_t col = 0; col < right.column_size(); col++) {
    result->add(matrix_vector_mul_op_op(left, right, row, col));
  }
  return result;
}

inline Complex matrix_vector_mul_op_mat(const Operation &left,
                                        const ComplexVectMatrix &right,
                                        const size_t row, const size_t col) {
  ComplexVectSplit vect_right;
  for (size_t m = 0; m < right.column_size(); m++) {
    vect_right.add(right.get(0, m));
  }
  return simd::cvsve(*simd::cvmul(*left.get(col), vect_right));
}

inline std::unique_ptr<ComplexVectSplit>
matrix_vector_mul_op_mat_row(const Operation &left,
                             const ComplexVectMatrix &right, const size_t row) {
  auto result = std::make_unique<ComplexVectSplit>();
  for (size_t col = 0; col < right.column_size(); col++) {
    result->add(matrix_vector_mul_op_mat(left, right, row, col));
  }
  return result;
}

/*
 * Outer product
 */
inline Complex outer_product_mat_mat(const ComplexVectMatrix &left,
                                     const ComplexVectMatrix &right,
                                     const size_t m, const size_t n) {
  return left.get(0, m) * std::conj(right.get(0, n));
}

inline std::unique_ptr<ComplexVectSplit>
outer_product_mat_mat_row(const ComplexVectMatrix &left,
                          const ComplexVectMatrix &right, const size_t row) {
  return simd::cvsmul(*right.get_row(0)->conj(), left.get(0, row));
}

/*
 * Scalar Product
 */
inline Complex scalar_product_mat_mat(const ComplexVectMatrix &left,
                                      const ComplexVectMatrix &right,
                                      const size_t m, const size_t n) {
  return left.get(m, n) * right.get(0, 0);
}

inline std::unique_ptr<ComplexVectSplit>
scalar_product_mat_mat_row(const ComplexVectMatrix &left,
                           const ComplexVectMatrix &right, const size_t row) {
  return simd::cvsmul(*left.get_row(row), right.get(0, 0));
}

/*
 * Sum
 */
inline Complex sum_mat_mat(const ComplexVectMatrix &left,
                           const ComplexVectMatrix &right, const size_t m,
                           const size_t n) {
  return left.get(m, n) + right.get(m, n);
}

inline std::unique_ptr<ComplexVectSplit>
sum_mat_mat_row(const ComplexVectMatrix &left, const ComplexVectMatrix &right,
                const size_t row) {
  return simd::cvadd(*left.get_row(row), *right.get_row(row));
}

inline Complex sum_op_op(const Operation &left, const Operation &right,
                         const size_t m, const size_t n) {
  return left.get(m, n) + right.get(m, n);
}

inline std::unique_ptr<ComplexVectSplit>
sum_op_op_row(const Operation &left, const Operation &right, const size_t row) {
  return simd::cvadd(*left.get(row), *right.get(row));
}

/*
 * Tensor product
 */
inline Complex tensor_product_mat_mat(const ComplexVectMatrix &left,
                                      const ComplexVectMatrix &right,
                                      const size_t m, const size_t n) {
  return left.get(m / right.row_size(), n / right.column_size()) *
         right.get(m % right.row_size(), n % right.column_size());
}

inline Complex tensor_product_op_mat(const Operation &left,
                                     const ComplexVectMatrix &right,
                                     const size_t m, const size_t n) {
  return left.get(m / right.row_size(), n / right.column_size()) *
         right.get(m % right.row_size(), n % right.column_size());
}

inline Complex tensor_product_op_op(const Operation &left,
                                    const Operation &right, const size_t m,
                                    const size_t n) {
  return left.get(m / right.row_size(), n / right.column_size()) *
         right.get(m % right.row_size(), n % right.column_size());
}

/*
 * Tensor product left-hand (lhe) and right-hand (rhe) elements, given the
 * left-hand matrix of the operation and the range.
 */
inline std::unique_ptr<ComplexVectSplit>
tplhe(const ComplexVectMatrix &left, const size_t row, const size_t row_size,
      const size_t right_row_size, const size_t right_column_size) {
  auto result = std::make_unique<ComplexVectSplit>();
  const auto m = row / right_row_size;
  auto left_row = left.get_row(m);
  for (size_t n = 0; n < row_size; n++) {
    result->add(left_row->get(n / right_column_size));
  }
  return result;
}

inline std::unique_ptr<ComplexVectSplit>
tplhe(const Operation &left, const size_t row, const size_t row_size,
      const size_t right_row_size, const size_t right_column_size) {
  auto result = std::make_unique<ComplexVectSplit>();
  const auto m = row / right_row_size;
  auto left_row = left.get(m);
  for (size_t n = 0; n < row_size; n++) {
    result->add(left_row->get(n / right_column_size));
  }
  return result;
}

inline std::unique_ptr<ComplexVectSplit>
tprhe(const ComplexVectMatrix &right, const size_t row, const size_t row_size,
      const size_t right_row_size, const size_t right_column_size) {
  auto result = std::make_unique<ComplexVectSplit>();
  const auto m = row % right_row_size;
  for (size_t i = 0; i < row_size; i++) {
    auto n = i % right_column_size;
    result->add(right.get(m, n));
  }
  return result;
}

inline std::unique_ptr<ComplexVectSplit>
tprhe(const Operation &right, const size_t row, const size_t row_size,
      const size_t right_row_size, const size_t right_column_size) {
  auto result = std::make_unique<ComplexVectSplit>();
  const auto m = row % right_row_size;
  for (size_t i = 0; i < row_size; i++) {
    auto n = i % right_column_size;
    result->add(right.get(m, n));
  }
  return result;
}

inline std::unique_ptr<ComplexVectSplit>
tensor_product_mat_mat_row(const ComplexVectMatrix &left,
                           const ComplexVectMatrix &right, const size_t row) {
  const auto right_row_size = right.row_size();
  const auto right_column_size = right.column_size();
  const auto final_row_size = left.row_size() * right_row_size;
  return simd::cvmul(
      *tplhe(left, row, final_row_size, right_row_size, right_column_size),
      *tprhe(right, row, final_row_size, right_row_size, right_column_size));
}

inline std::unique_ptr<ComplexVectSplit>
tensor_product_op_mat_row(const Operation &left, const ComplexVectMatrix &right,
                          const size_t row) {
  const auto right_row_size = right.row_size();
  const auto right_column_size = right.column_size();
  const auto final_row_size = left.row_size() * right_row_size;
  return simd::cvmul(
      *tplhe(left, row, final_row_size, right_row_size, right_column_size),
      *tprhe(right, row, final_row_size, right_row_size, right_column_size));
}

inline std::unique_ptr<ComplexVectSplit>
tensor_product_op_op_row(const Operation &left, const Operation &right,
                         const size_t row) {
  const auto right_row_size = right.row_size();
  const auto right_column_size = right.column_size();
  const auto final_row_size = left.row_size() * right_row_size;
  return simd::cvmul(
      *tplhe(left, row, final_row_size, right_row_size, right_column_size),
      *tprhe(right, row, final_row_size, right_row_size, right_column_size));
}

inline std::unique_ptr<ComplexVectSplit>
vv_tensor_product_mat_mat_row(const ComplexVectMatrix &left,
                              const ComplexVectMatrix &right,
                              const size_t row) {
  auto result = std::make_unique<ComplexVectSplit>();
  const auto right_split = *right.get_row(0);
  for (size_t m = 0; m < left.column_size(); m++) {
    const auto v = *simd::cvsmul(right_split, left.get(0, m));
    result->insert(v.real(), v.imag());
  }
  return result;
}

inline std::unique_ptr<ComplexVectSplit>
vv_tensor_product_op_mat_row(const Operation &left,
                             const ComplexVectMatrix &right, const size_t row) {
  auto result = std::make_unique<ComplexVectSplit>();
  const auto right_split = *right.get_row(0);
  for (size_t m = 0; m < left.column_size(); m++) {
    const auto v = *simd::cvsmul(right_split, left.get(0, m));
    result->insert(v.real(), v.imag());
  }
  return result;
}

inline std::unique_ptr<ComplexVectSplit>
vv_tensor_product_op_op_row(const Operation &left, const Operation &right,
                            const size_t row) {
  auto result = std::make_unique<ComplexVectSplit>();
  const auto right_split = *right.get(0);
  for (size_t m = 0; m < left.column_size(); m++) {
    const auto v = *simd::cvsmul(right_split, left.get(0, m));
    result->insert(v.real(), v.imag());
  }
  return result;
}

/*
 * Implementations
 */

std::unique_ptr<LazyOperation>
AlgebraEngine::conjugate_transpose(const ComplexVectMatrix &mat) {
  return std::make_unique<LazyOperation>(
      mat, ComplexVectMatrix(), conjugate_transpose_lazy,
      conjugate_transpose_row, mat.column_size(), mat.row_size());
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
  return std::make_unique<LazyOperation>(
      mat_left, mat_right, matrix_multiplication_mat_mat,
      matrix_multiplication_mat_mat_row, mat_left.row_size(),
      mat_right.column_size());
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_exp(const ComplexVectMatrix &mat, const size_t folds) {
  if (mat.row_size() != mat.column_size()) {
    throw std::invalid_argument(
        "Can not compute matrix exponentiantion: input is not a square matrix");
  }
  if (folds == 0) {
    return LazyOperation::identity(mat.row_size());
  }
  auto lazy = std::make_unique<LazyOperation>(mat);
  const auto row_size = mat.row_size();
  const auto col_size = mat.column_size();
  for (int i = 0; i < folds; i++) {
    lazy->append(mat, matrix_multiplication_op_mat,
                 matrix_multiplication_op_mat_row, row_size, col_size);
  }
  return lazy;
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(const ComplexVectMatrix &mat,
                                     const ComplexVectMatrix &vect) {
  return std::make_unique<LazyOperation>(mat, vect, matrix_vector_mul_mat_mat,
                                         matrix_vector_mul_mat_mat_row, 1,
                                         vect.column_size());
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(const LazyOperation &mat,
                                     const ComplexVectMatrix &vect) {
  auto result = std::make_unique<LazyOperation>(mat);
  result->append(vect, matrix_vector_mul_op_mat, matrix_vector_mul_op_mat_row,
                 1, vect.column_size());
  return result;
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(const LazyOperation &mat,
                                     const LazyOperation &vect) {
  auto result = std::make_unique<LazyOperation>(mat);
  result->append(vect, matrix_vector_mul_op_op, matrix_vector_mul_op_op_row, 1,
                 vect.column_size());
  return result;
}

std::unique_ptr<LazyOperation>
AlgebraEngine::outer_product(const ComplexVectMatrix &mat_left,
                             const ComplexVectMatrix &mat_right) {
  return std::make_unique<LazyOperation>(
      mat_left, mat_right, outer_product_mat_mat, outer_product_mat_mat_row,
      mat_left.column_size(), mat_right.column_size());
}

std::unique_ptr<LazyOperation>
AlgebraEngine::scalar_product(const ComplexVectMatrix &mat, const Complex &k) {
  return std::make_unique<LazyOperation>(
      mat, ComplexVectMatrix(k), scalar_product_mat_mat,
      scalar_product_mat_mat_row, mat.row_size(), mat.column_size());
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matsum(const ComplexVectMatrix &left,
                      const ComplexVectMatrix &right) {
  size_t left_row_size = left.row_size();
  size_t left_column_size = left.column_size();
  if (left_row_size != right.row_size() ||
      left_column_size != right.column_size()) {
    throw std::invalid_argument("Matrix sizes do not match");
  }
  return std::make_unique<LazyOperation>(left, right, sum_mat_mat,
                                         sum_mat_mat_row, left_row_size,
                                         left_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matsum(const ComplexVectMatrix &left,
                      const LazyOperation &right) {
  size_t left_row_size = left.row_size();
  size_t left_column_size = left.column_size();
  if (left_row_size != right.row_size() ||
      left_column_size != right.column_size()) {
    throw std::invalid_argument("Matrix sizes do not match");
  }
  return std::make_unique<LazyOperation>(left, right, sum_op_op, sum_op_op_row,
                                         left_row_size, left_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matsum(const LazyOperation &left, const LazyOperation &right) {
  size_t left_row_size = left.row_size();
  size_t left_column_size = left.column_size();
  if (left_row_size != right.row_size() ||
      left_column_size != right.column_size()) {
    throw std::invalid_argument("Matrix sizes do not match");
  }
  auto r = std::make_unique<LazyOperation>(left);
  r->append(right, sum_op_op, sum_op_op_row, left_row_size, left_column_size);
  return r;
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const ComplexVectMatrix &mat_left,
                              const ComplexVectMatrix &mat_right) {
  if (mat_left.row_size() != 1 || mat_right.row_size() != 1) {
    return std::make_unique<LazyOperation>(
        mat_left, mat_right, tensor_product_mat_mat, tensor_product_mat_mat_row,
        mat_left.row_size() * mat_right.row_size(),
        mat_left.column_size() * mat_right.column_size());
  } else {
    return std::make_unique<LazyOperation>(
        mat_left, mat_right, tensor_product_mat_mat,
        vv_tensor_product_mat_mat_row,
        mat_left.row_size() * mat_right.row_size(),
        mat_left.column_size() * mat_right.column_size());
  }
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const ComplexVectMatrix &mat_left,
                              const LazyOperation &op_right) {
  if (mat_left.row_size() != 1 || op_right.row_size() != 1) {
    return std::make_unique<LazyOperation>(
        mat_left, op_right, tensor_product_op_op, tensor_product_op_op_row,
        mat_left.row_size() * op_right.row_size(),
        mat_left.column_size() * op_right.column_size());
  } else {
    return std::make_unique<LazyOperation>(
        mat_left, op_right, tensor_product_op_op, vv_tensor_product_op_op_row,
        mat_left.row_size() * op_right.row_size(),
        mat_left.column_size() * op_right.column_size());
  }
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const LazyOperation &left,
                              const ComplexVectMatrix &right) {
  auto result = std::make_unique<LazyOperation>(left);
  if (left.row_size() != 1 || right.row_size() != 1) {
    result->append(right, tensor_product_op_mat, tensor_product_op_mat_row,
                   left.row_size() * right.row_size(),
                   left.column_size() * right.column_size());
  } else {
    result->append(right, tensor_product_op_mat, vv_tensor_product_op_mat_row,
                   left.row_size() * right.row_size(),
                   left.column_size() * right.column_size());
  }
  return result;
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const LazyOperation &left,
                              const LazyOperation &right) {
  auto result = std::make_unique<LazyOperation>(left);
  if (left.row_size() != 1 || right.row_size() != 1) {
    result->append(right, tensor_product_op_op, tensor_product_op_op_row,
                   left.row_size() * right.row_size(),
                   left.column_size() * right.column_size());
  } else {
    result->append(right, tensor_product_op_op, vv_tensor_product_op_op_row,
                   left.row_size() * right.row_size(),
                   left.column_size() * right.column_size());
  }
  return result;
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const ComplexVectMatrix &mat,
                              const size_t folds) {
  if (folds == 1) {
    return std::make_unique<LazyOperation>(mat);
  }

  auto lazy = std::make_unique<LazyOperation>(mat);
  for (size_t _ = 1; _ < folds; _++) {
    lazy->append(mat, tensor_product_op_mat, tensor_product_op_mat_row,
                 lazy->row_size() * mat.row_size(),
                 lazy->column_size() * mat.column_size());
  }
  return lazy;
}

bool AlgebraEngine::is_unitary(const ComplexVectMatrix &mat) {
  if (mat.row_size() != mat.column_size()) {
    return false;
  }

  auto lazy = std::make_unique<LazyOperation>(mat);
  auto mat_dagger = AlgebraEngine::conjugate_transpose(mat);
  lazy->append(*mat_dagger, matrix_multiplication_op_op,
               matrix_multiplication_op_op_row, lazy->row_size(),
               mat_dagger->column_size());

  for (size_t m = 0; m < lazy->row_size(); m++) {
    if (!approx_equal(lazy->get(m, m), 1)) {
      return false;
    }
    if (!approx_equal(simd::cvsve(*lazy->get(m)), 1)) {
      return false;
    }
  }
  return true;
}
