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

#ifndef ALGEBRA_ENGINE_H
#define ALGEBRA_ENGINE_H

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"

#include <memory>

class AlgebraEngine {
public:
  AlgebraEngine() = delete;

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  conjugate_transpose(const ComplexVectMatrix &mat);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  inner_product(const ComplexVectMatrix &vect_left,
                const ComplexVectMatrix &vect_right);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  matrix_multiplication(const ComplexVectMatrix &mat_left,
                        const ComplexVectMatrix &mat_right);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  matrix_exp(const ComplexVectMatrix &mat, const size_t folds);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  matrix_vector_product(const ComplexVectMatrix &mat,
                        const ComplexVectMatrix &vect);

  static void matrix_vector_product(std::unique_ptr<LazyOperation> &mat,
                                    const ComplexVectMatrix &vect);

  static void matrix_vector_product(std::unique_ptr<LazyOperation> &mat,
                                    const LazyOperation &vect);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  outer_product(const ComplexVectMatrix &mat_left,
                const ComplexVectMatrix &mat_right);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  scalar_product(const ComplexVectMatrix &mat, const Complex &k);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  sum(const ComplexVectMatrix &mat_left, const ComplexVectMatrix &mat_right);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  sum(const ComplexVectMatrix &mat_left, const LazyOperation &mat_right);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  tensor_product(const ComplexVectMatrix &mat_left,
                 const ComplexVectMatrix &mat_right);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  tensor_product(const ComplexVectMatrix &left, const LazyOperation &right);

  /*
   * Calculates the tensor product between the two input vectors, adding a
   * `Operation` to the `LazyOperation`.
   */
  static void tensor_product(std::unique_ptr<LazyOperation> &left,
                             const ComplexVectMatrix &right);

  [[nodiscard]] static std::unique_ptr<LazyOperation>
  tensor_product(const ComplexVectMatrix &mat, const size_t folds);

  [[nodiscard]] static bool is_unitary(const ComplexVectMatrix &mat);
};

#endif // !ALGEBRA_ENGINE_H
