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

#include "circuit_engine.h"
#include "gate_engine.h"
#include "hilbert_namespace_test.h"
#include "qubit.h"
#include "state_vector.h"
#include <numbers>
#include <vector>

bool it_should_compute_qft() {
  // Given
  auto ket_0_q = Qubit(1, 0);
  auto ket_1_q = Qubit(0, 1);
  auto state_1 = StateVector({ket_0_q, ket_0_q});
  auto state_2 = StateVector({ket_0_q, ket_1_q});

  // When
  auto result_1 = CircuitEngine::qft(state_1);
  auto result_2 = CircuitEngine::qft(state_2);

  // Then
  auto ket_a_1 = Qubit(1 / std::sqrt(2), 1 / std::sqrt(2));
  auto expected_1 = StateVector({ket_a_1, ket_a_1});
  bool case_1_ok = expected_1 == *result_1;

  auto ket_a_2 = Qubit(1 / std::sqrt(2), -1 / std::sqrt(2));
  auto ket_b_2 = Qubit(1 / std::sqrt(2), Complex(0, 1 / std::sqrt(2)));
  auto expected_2 = StateVector({ket_a_2, ket_b_2});
  bool case_2_ok = expected_2 == *result_2;

  return case_1_ok && case_2_ok;
}

bool it_should_compute_qft_and_inverse() {
  // Given
  auto ket_0_q = Qubit(1, 0);
  auto ket_1_q = Qubit(0, 1);
  auto state = StateVector({ket_0_q, ket_1_q, ket_0_q, ket_0_q, ket_1_q});

  // When
  auto state_qft = CircuitEngine::qft(state);
  auto state_iqft = CircuitEngine::inverse_qft(*state_qft);

  // Then
  return state == *state_iqft;
}

bool it_should_compute_qpe() {
  // Given
  auto phase = 2 * std::numbers::pi / 5;
  auto u = GateEngine::phase_shift_gate(phase);
  auto v = Qubit::ket_1();

  // When
  CircuitEngine::qpe(*v, *u);
}

int main() {
  int total = 0;
  int failed = 0;

  run_test("it_should_compute_qft", it_should_compute_qft, failed, total);

  run_test("it_should_compute_qft_and_inverse",
           it_should_compute_qft_and_inverse, failed, total);

  run_test("it_should_compute_qpe", it_should_compute_qpe, failed, total);

  test_resumen(failed, total);
  return failed == 0 ? 0 : 1;
}
