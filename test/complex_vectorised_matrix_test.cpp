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

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace_test.h"
#include <memory>

bool it_should_create_vectorised_matrix() {
  // When
  const auto m_vectorised = ComplexVectMatrix::identity_2x2();

  // Then
  return verify_identity_matrix(*m_vectorised, 2);
}

bool it_should_compare_vect_matrices() {
  // Given
  auto vect_a = *ComplexVectMatrix::identity_2x2();
  auto vect_b = *ComplexVectMatrix::hadamard_2x2();

  // When
  auto a_eq_a = vect_a == vect_a;
  auto b_eq_b = vect_b == vect_b;
  auto a_eq_b = vect_a == vect_b;
  auto b_eq_a = vect_b == vect_a;

  // Then
  return a_eq_a && b_eq_b && !a_eq_b && !b_eq_a;
}

bool it_should_split_vect_matrix() {
  // Given
  auto mat = ComplexVectMatrix::pauli_y();

  // When
  auto mat_split = mat->split();

  // Then
  std::vector<__complex_precision> real_exp = {0, 0, 0, 0};
  std::vector<__complex_precision> imag_exp = {0, -1, 1, 0};
  auto real_act = *mat_split->real();
  auto imag_act = *mat_split->imag();
  return real_exp == real_act && imag_exp == imag_act;
}

int main() {
  int total = 0;
  int failed = 0;

  run_test("it_should_create_vectorised_matrix",
           it_should_create_vectorised_matrix, failed, total);

  run_test("it_should_compare_vect_matrices", it_should_compare_vect_matrices,
           failed, total);

  run_test("it_should_split_vect_matrix", it_should_split_vect_matrix, failed,
           total);

  test_resumen(failed, total);
  return failed == 0 ? 0 : 1;
}
