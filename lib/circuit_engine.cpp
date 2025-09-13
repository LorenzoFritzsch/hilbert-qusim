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
#include "algebra_engine.h"
#include "gate_engine.h"
#include "hilbert_namespace.h"
#include "qubit.h"
#include "state_vector.h"
#include <memory>

std::unique_ptr<StateVector> CircuitEngine::qft(const StateVector &j) {
  std::vector<Qubit> result(j.size());

  auto last_index = result.size() - 1;
  for (size_t i = 0; i < j.size(); i++) {
    auto j_k = GateEngine::hadamard(j.get(i));
    for (size_t k_next = i + 1; k_next < j.size(); k_next++) {
      j_k = GateEngine::controlled_u(*j_k, j.get(k_next),
                                     *GateEngine::r_k(k_next + 1));
    }
    result[last_index - i] = *j_k;
  }

  return std::make_unique<StateVector>(result);
}

std::unique_ptr<StateVector> CircuitEngine::inverse_qft(const StateVector &k) {
  std::vector<Qubit> result(k.size());

  auto swapped(k.get());
  std::reverse(swapped.begin(), swapped.end());

  auto last_index = swapped.size() - 1;
  for (int i = last_index; i >= 0; i--) {
    auto k_i = swapped[i];
    for (size_t j = last_index; j > i; j--) {
      k_i = *GateEngine::controlled_u(k_i, result[j],
                                      *GateEngine::r_k(j + 1, true));
    }
    result[i] = *GateEngine::hadamard(k_i);
  }

  return std::make_unique<StateVector>(result);
}

// TODO: remove
#include "../test/hilbert_namespace_test.h"

__complex_precision
CircuitEngine::qpe(const Qubit &v, const ComplexVectMatrix &u, const int t) {
  mxout(u, "U");
  StateVector r1(t);
  for (size_t i = 0; i < t; i++) {
    auto control = r1[i];
    control = *GateEngine::hadamard(control);
    auto u_k = *AlgebraEngine::matrix_exp(u, 1 << i)->to_matrix();
    mxout(u_k, "U" + std::to_string(i));
    auto state = GateEngine::controlled_u_stv(v, control, u_k);
    mxout(*state->to_matrix(), "State after U" + std::to_string(i));
    r1[i] = *GateEngine::trout_control(*state);
  }
  svout(r1, "State before iQFT");
  auto s = inverse_qft(r1);
  svout(*s, "State after iQFT");
}
