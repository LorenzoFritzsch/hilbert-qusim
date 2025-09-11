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

#include "algebra_engine.h"
#include "lazy_operation.h"
#include "qubit.h"
#include <cstddef>
#include <stdexcept>
#include <vector>

/*
 * This class represents a full state-vector, where each element is a `Qubit`.
 */
class StateVector final {
public:
  explicit StateVector(const std::vector<Qubit> state_vector)
      : state_vector_(state_vector) {
    if (state_vector_.size() == 0) {
      throw std::invalid_argument("State vector must have size > 0");
    }
  }

  StateVector(const size_t size) : state_vector_(size) {}

  bool operator==(const StateVector &other) const {
    return state_vector_ == other.state_vector_;
  }

  const Qubit &operator[](size_t index) const { return state_vector_[index]; }
  Qubit get(size_t i) const { return state_vector_.at(i); }

  Qubit &operator[](size_t index) { return state_vector_[index]; }
  std::vector<Qubit> get() const { return state_vector_; }

  void push_back(const Qubit &q) { state_vector_.push_back(q); }

  std::unique_ptr<LazyOperation> to_vector() const {
    if (state_vector_.size() == 1) {
      return std::make_unique<LazyOperation>(*state_vector_[0].to_vector());
    }
    auto lazy = AlgebraEngine::tensor_product(*state_vector_[0].to_vector(),
                                              *state_vector_[1].to_vector());
    for (size_t i = 2; i < state_vector_.size(); i++) {
      AlgebraEngine::tensor_product(*lazy, *state_vector_[i].to_vector());
    }
    return lazy;
  }

  size_t size() const { return state_vector_.size(); }

private:
  std::vector<Qubit> state_vector_;
};

#endif // !STATE_VECTOR_H
