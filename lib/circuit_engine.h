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

#include "qubit.h"
#include <vector>

class CircuitEngine {
public:
  CircuitEngine() = delete;

  static std::vector<Qubit> qft(const std::vector<Qubit> j);

  static std::vector<Qubit> inverse_qft(const std::vector<Qubit> k);
};

#endif // !CIRCUIT_ENGINE_H
