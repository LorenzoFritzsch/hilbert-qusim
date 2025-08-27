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
#include "qubit.h"
#include <memory>
#include <stdexcept>

bool it_should_create_qubit() {
  // Given
  constexpr auto alpha = Complex(1, 0);
  constexpr auto beta = Complex(0, 0);

  // When
  const auto qubit = std::make_unique<Qubit>(alpha, beta);

  // Then
  return *ComplexVectMatrix::ket_0() == *qubit->to_vector();
}

bool it_should_not_create_qubit_with_invalid_alpha_beta() {
  // Given
  constexpr auto alpha = Complex(1, 0);
  constexpr auto beta = Complex(1, 0);

  // When - Then
  try {
    const auto qubit = Qubit(alpha, beta);
    return false;
  } catch (const std::invalid_argument &e) {
    return true;
  }
}

bool it_should_create_qubit_cvm() {
  // Given
  auto qubit_vect = ComplexVectMatrix::ket_0();

  // When
  const auto qubit = std::make_unique<Qubit>(*qubit_vect);

  // Then
  return *ComplexVectMatrix::ket_0() == *qubit->to_vector();
}

bool it_should_not_create_qubit_invalid_cvm() {
  // Given
  auto invalid_opmember = ComplexVectMatrix::hadamard_2x2();

  // When - Then
  try {
    const auto qubit = std::make_unique<Qubit>(*invalid_opmember);
    return false;
  } catch (const std::invalid_argument &e) {
    return true;
  }
}

bool it_should_not_create_qubit_opmember_invalid_alpha_beta() {
  // Given
  auto qubit_vect = std::make_unique<ComplexVectMatrix>(ComplexVector({1, 1}));

  // When - Then
  try {
    const auto qubit = std::make_unique<Qubit>(*qubit_vect);
    return false;
  } catch (const std::invalid_argument &e) {
    return true;
  }
}

int main() {
  int total = 0;
  int failed = 0;

  run_test("it_should_create_qubit", it_should_create_qubit, failed, total);

  run_test("it_should_not_create_qubit_with_invalid_alpha_beta",
           it_should_not_create_qubit_with_invalid_alpha_beta, failed, total);

  run_test("it_should_create_qubit_opmember", it_should_create_qubit_cvm,
           failed, total);

  run_test("it_should_not_create_qubit_invalid_opmember",
           it_should_not_create_qubit_invalid_cvm, failed, total);

  run_test("it_should_not_create_qubit_opmember_invalid_alpha_beta",
           it_should_not_create_qubit_opmember_invalid_alpha_beta, failed,
           total);

  test_resumen(failed, total);
  return failed == 0 ? 0 : 1;
}
