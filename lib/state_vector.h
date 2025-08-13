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

#ifndef STATE_VECTOR_H
#define STATE_VECTOR_H

#include "qubit.h"
#include <cstddef>
#include <vector>

class StateVector {
public:
  explicit StateVector(const std::vector<Qubit> state_vector)
      : state_vector_(state_vector) {}

  bool operator==(const StateVector &other) const {
    return state_vector_ == other.state_vector_;
  }

  Qubit get(size_t i) const { return state_vector_.at(i); }

  std::vector<Qubit> get() const { return state_vector_; }

  size_t size() const { return state_vector_.size(); }

private:
  const std::vector<Qubit> state_vector_;
};

#endif // !STATE_VECTOR_H
