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
#include "gate_engine.h"
#include "hilbert_namespace_test.h"
#include <cmath>
#include <memory>

bool it_should_apply_gate() {
  // Given
  auto gate = std::make_unique<ComplexVectMatrix>(pauli_x);
  auto state = std::make_unique<ComplexVectMatrix>(ket_0);

  // When
  const auto result = GateEngine::apply_gate(*gate, *state);

  // Then
  return are_matrices_equal(ComplexVectMatrix(ket_1), *result);
}

bool it_should_apply_controlled_gate() {
  // Given
  auto target = std::make_unique<Qubit>(1, 0);
  auto control = std::make_unique<Qubit>(0, 1);
  auto gate = std::make_unique<ComplexVectMatrix>(pauli_x);

  // When
  auto result = GateEngine::controlled_u(*target, *control, *gate);

  // Then
  const auto expected = ket_1;
  return are_matrices_equal(ComplexVectMatrix(ket_1), *result->to_vector());
}

bool it_should_apply_hadamard() {
  // Given
  auto qubit = std::make_unique<Qubit>(1, 0);

  // When
  auto result = GateEngine::hadamard(*qubit);

  // Then
  auto alpha_beta_exp = 1 / std::sqrt(2);
  const auto expected = std::make_unique<Qubit>(alpha_beta_exp, alpha_beta_exp);
  return are_matrices_equal(*expected->to_vector(), *result->to_vector());
}

int main() {
  int total = 0;
  int failed = 0;

  run_test("it_should_apply_gate", it_should_apply_gate, failed, total);

  run_test("it_should_apply_controlled_gate", it_should_apply_controlled_gate,
           failed, total);

  run_test("it_should_apply_hadamard", it_should_apply_hadamard, failed, total);

  test_resumen(failed, total);
  return failed == 0 ? 0 : 1;
}
