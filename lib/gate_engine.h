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

#ifndef GATE_ENGINE_H
#define GATE_ENGINE_H

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include "qubit.h"
#include <memory>
#include <numbers>

class GateEngine {
public:
  GateEngine() = delete;

  /*
   * Traces out the control qubit given the |control, target> 4-dimensional
   * state vector.
   */
  [[nodiscard]] static std::unique_ptr<Qubit>
  trout_control(const LazyOperation &s);

  /*
   * Traces out the target qubit given the |control, target> 4-dimensional
   * state vector.
   */
  [[nodiscard]] static std::unique_ptr<Qubit>
  trout_target(const LazyOperation &s);

  /*
   * Returns the result of applying the given gate to the state.
   */
  [[nodiscard]] static std::unique_ptr<LazyOperation>
  apply_gate(const ComplexVectMatrix &gate, const ComplexVectMatrix &state);

  /*
   * Applies a controlled-U gate and returns the resulting target qubit.
   */
  [[nodiscard]] static std::unique_ptr<Qubit>
  controlled_u(const Qubit &target, const Qubit &control,
               const ComplexVectMatrix &u);

  /*
   * Given the input states |control> and |target>, both bipartite states,
   * applies a controlled-U gate on the first qubits, and returns the resulting
   * state vector.
   * As the input states have both dimensions 4, the output will be a
   * 16-dimensional state vector.
   */
  [[nodiscard]] static std::unique_ptr<LazyOperation>
  controlled_u(std::unique_ptr<LazyOperation> control,
               std::unique_ptr<LazyOperation> target,
               const ComplexVectMatrix &u);

  /*
   * Applies a controlled-U gate and returns the resulting |control, target>
   * 4-dimensional state vector.
   */
  [[nodiscard]] static std::unique_ptr<LazyOperation>
  controlled_u_stv(const Qubit &target, const Qubit &control,
                   const ComplexVectMatrix &u);

  /*
   * Applies a Hadamard gate on the input and returns the resulting qubit.
   */
  [[nodiscard]] static std::unique_ptr<Qubit> hadamard(const Qubit &qubit);

  /*
   * Returns the 2x2 phase shift gate matrix for the given input phase.
   */
  [[nodiscard]] inline static std::unique_ptr<ComplexVectMatrix>
  phase_shift_gate(const __complex_precision &p, bool inverse = false) {
    auto rotation_real = std::cos(p);
    auto rotation_imag = std::sin(p);
    return std::make_unique<ComplexVectMatrix>(
        ComplexMatrix({{1, 0}, {0, Complex(rotation_real, rotation_imag)}}));
  }

  /*
   * Returns the 2x2 parameterised phase shift gate matrix with rotation 2pi /
   * 2^k.
   */
  [[nodiscard]] inline static std::unique_ptr<ComplexVectMatrix>
  r_k(const int k, bool inverse = false) {
    auto rotation = 2 * std::numbers::pi / std::pow(2, k);
    if (inverse) {
      rotation = -rotation;
    }
    return phase_shift_gate(rotation);
  }
};

#endif // !GATE_ENGINE_H
