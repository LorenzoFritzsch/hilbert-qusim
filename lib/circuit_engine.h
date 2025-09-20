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

#ifndef CIRCUIT_ENGINE_H
#define CIRCUIT_ENGINE_H

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "qubit.h"
#include "state_vector.h"
#include <memory.h>
#include <memory>

class CircuitEngine {
public:
  CircuitEngine() = delete;

  /*
   * Given any type of StateVector, it computes the Quantum Fourier Transform
   * circuit and returns a Lazy StateVector.
   */
  [[nodiscard]] static std::unique_ptr<StateVector> qft(const StateVector &j);

  /*
   * Given any type of StateVector, it computes the Inverse Quantum Fourier
   * Transform circuit and returns a Lazy StateVector.
   */
  [[nodiscard]] static std::unique_ptr<StateVector>
  inverse_qft(const StateVector &k);

  [[nodiscard]] static __complex_precision
  qpe(const Qubit &v, const ComplexVectMatrix &u, const int t = 8);
};

#endif // !CIRCUIT_ENGINE_H
