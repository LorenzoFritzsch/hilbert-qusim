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
#include "complex_vectorised_matrix.h"
#include "lazy_operation.h"
#include "qubit.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

class StateVector final {
public:
  enum Type { Qbit, Lazy };

  explicit StateVector(const std::vector<Qubit> state_vector)
      : StateVector(state_vector, Qbit) {}

  /*
   * Initialises n wires to |0>
   */
  StateVector(const size_t n, const Type &type)
      : StateVector(
            type == Qbit
                ? std::variant<std::vector<Qubit>, std::vector<LazyOperation>>(
                      std::vector<Qubit>(n))
                : std::variant<std::vector<Qubit>, std::vector<LazyOperation>>(
                      std::vector<LazyOperation>(
                          n, LazyOperation(*ComplexVectMatrix::ket_0()))),
            type) {}

  std::variant<Qubit, LazyOperation> get(size_t i) const {
    switch (type_) {
    case Qbit:
      return std::get<std::vector<Qubit>>(state_vector_).at(i);
    case Lazy:
      return std::get<std::vector<LazyOperation>>(state_vector_).at(i);
    }
    throw std::logic_error("Unexpected StateVector::Type");
  }

  const std::variant<std::vector<Qubit>, std::vector<LazyOperation>>
  get() const {
    return state_vector_;
  }

  void push_back(const Qubit &q) {
    if (type_ != Qbit) {
      throw std::logic_error(
          "Can not push a Qubit in a LazyOperation type State Vector");
    }
    std::get<std::vector<Qubit>>(state_vector_).push_back(q);
  }

  void push_back(const LazyOperation &q) {
    if (type_ != Lazy) {
      throw std::logic_error(
          "Can not push a LazyOperation in a Qubit type State Vector");
    }
    std::get<std::vector<LazyOperation>>(state_vector_).push_back(q);
  }

  void insert(const Qubit &e, const size_t i) {
    if (type_ != Qbit) {
      throw std::logic_error(
          "Can not push a Qubit in a LazyOperation type State Vector");
    }
    std::get<std::vector<Qubit>>(state_vector_)[i] = e;
  }

  void insert(LazyOperation &e, const size_t i) {
    if (type_ != Lazy) {
      throw std::logic_error(
          "Can not push a LazyOperation in a Qubit type State Vector");
    }
    std::get<std::vector<LazyOperation>>(state_vector_)[i] = e;
  }

  void reverse() {
    switch (type_) {
    case Qbit: {
      auto &vect = std::get<std::vector<Qubit>>(state_vector_);
      std::reverse(vect.begin(), vect.end());
      break;
    }
    case Lazy: {
      auto &vect = std::get<std::vector<LazyOperation>>(state_vector_);
      std::reverse(vect.begin(), vect.end());
      break;
    }
    }
  }

  std::unique_ptr<LazyOperation> to_vector() const {
    switch (type_) {
    case Qbit: {
      if (size() == 1) {
        return std::make_unique<LazyOperation>(
            *std::get<std::vector<Qubit>>(state_vector_)[0].to_vector());
      }

      const auto &vect = std::get<std::vector<Qubit>>(state_vector_);
      auto lazy = AlgebraEngine::tensor_product(*vect[0].to_vector(),
                                                *vect[1].to_vector());
      for (size_t i = 2; i < vect.size(); i++) {
        lazy = AlgebraEngine::tensor_product(*lazy, *vect[i].to_vector());
      }
      return lazy;
    }
    case Lazy:
      if (size() == 1) {
        return std::make_unique<LazyOperation>(
            std::get<std::vector<LazyOperation>>(state_vector_)[0]);
      }

      const auto &vect = std::get<std::vector<LazyOperation>>(state_vector_);
      auto lazy = AlgebraEngine::tensor_product(vect[0], vect[1]);
      for (size_t i = 2; i < vect.size(); i++) {
        lazy = AlgebraEngine::tensor_product(*lazy, vect[i]);
      }
      return lazy;
    }
  }

  size_t size() const {
    switch (type_) {
    case Qbit:
      return std::get<std::vector<Qubit>>(state_vector_).size();
    case Lazy:
      return std::get<std::vector<LazyOperation>>(state_vector_).size();
    }
    throw std::logic_error("Unexpected StateVector::Type");
  }

  Type type() const { return type_; }

  /* Following methods don't really align with basic quantum principles but are
   * quite useful in different scenarios, and for performance reasons. */

  bool operator==(const StateVector &other) const {
    if (type_ != other.type_) {
      return false;
    }
    switch (type_) {
    case Qbit:
      return std::get<std::vector<Qubit>>(state_vector_) ==
             std::get<std::vector<Qubit>>(other.state_vector_);
    case Lazy:
      return std::get<std::vector<LazyOperation>>(state_vector_) ==
             std::get<std::vector<LazyOperation>>(other.state_vector_);
      break;
    }
    throw std::logic_error("Unexpected StateVector::Type");
  }

private:
  const Type type_;
  std::variant<std::vector<Qubit>, std::vector<LazyOperation>> state_vector_;

  StateVector(std::variant<std::vector<Qubit>, std::vector<LazyOperation>> vect,
              const Type &type)
      : type_(type), state_vector_(std::move(vect)) {
    if (size() == 0) {
      throw std::invalid_argument("State vector must have size > 0");
    }
  }
};

#endif // !STATE_VECTOR_H
