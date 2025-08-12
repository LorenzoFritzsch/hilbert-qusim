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

#include "lazy_operation.h"
#include "qubit.h"
#include <memory>

class GateEngine {
public:
  GateEngine() = delete;

  static std::unique_ptr<LazyOperation>
  apply_gate(const ComplexVectMatrix &gate, const ComplexVectMatrix &state);

  static std::unique_ptr<Qubit> controlled_u(const Qubit &target,
                                             const Qubit &control,
                                             const ComplexVectMatrix &u);

  static std::unique_ptr<Qubit> hadamard(const Qubit &qubit);

  static std::unique_ptr<ComplexVectMatrix> r_k(const int k,
                                                bool inverse = false);
};

#endif // !GATE_ENGINE_H
