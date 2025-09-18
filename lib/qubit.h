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

#ifndef QUBIT_H
#define QUBIT_H
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include <complex>
#include <memory>
#include <stdexcept>

class Qubit final {
public:
  Qubit(const Complex alpha, const Complex beta) {
    auto n = norm(alpha, beta);
    if (!approx_equal(n, 1)) {
      throw std::invalid_argument(
          "Qubit must be normalised (norm = " + std::to_string(n.real()) + ")");
    }
    this->alpha = alpha;
    this->beta = beta;
  }

  explicit Qubit(const ComplexVectMatrix &vect) {
    if (vect.row_size() != 1 || vect.column_size() != 2) {
      throw std::invalid_argument("Input must represent a 2 elements vector");
    }
    auto alpha = vect.get(0, 0);
    auto beta = vect.get(0, 1);
    auto n = norm(alpha, beta);
    if (!approx_equal(n, 1)) {
      throw std::invalid_argument(
          "Qubit must be normalised (norm = " + std::to_string(n.real()) + ")");
    }
    this->alpha = alpha;
    this->beta = beta;
  }

  explicit Qubit(const LazyOperation &op) {
    if (op.row_size() != 1 || op.column_size() != 2) {
      throw std::invalid_argument("Input must represent a 2 elements vector");
    }
    auto alpha = op.get(0, 0);
    auto beta = op.get(0, 1);
    auto n = norm(alpha, beta);
    if (!approx_equal(n, 1)) {
      throw std::invalid_argument(
          "Qubit must be normalised (norm = " + std::to_string(n.real()) + ")");
    }
    this->alpha = alpha;
    this->beta = beta;
  }

  Qubit() {
    this->alpha = 1;
    this->beta = 0;
  }

  Qubit operator*=(const __complex_precision &c) const {
    auto new_alpha = alpha * c;
    auto new_beta = beta * c;
    auto n = norm(new_alpha, new_beta);
    return Qubit(new_alpha / n, new_beta / n);
  }

  [[nodiscard]] static std::unique_ptr<Qubit> ket_0() {
    return std::make_unique<Qubit>(1, 0);
  }

  [[nodiscard]] static std::unique_ptr<Qubit> ket_1() {
    return std::make_unique<Qubit>(0, 1);
  }

  [[nodiscard]] std::unique_ptr<ComplexVectMatrix> to_vector() const {
    return std::make_unique<ComplexVectMatrix>(ComplexVector({alpha, beta}));
  }

  /*
   * Even though these methods are quite against basic quantum information
   * theory concepts, they are rather useful in testing scenarios or to improve
   * performance.
   */

  bool operator==(const Qubit &other) const {
    return approx_equal(alpha, other.alpha) && approx_equal(beta, other.beta);
  }

  bool operator!=(const Qubit &other) const {
    return !approx_equal(alpha, other.alpha) && approx_equal(beta, other.beta);
  }

private:
  Complex alpha;
  Complex beta;
};

#endif // !QUBIT_H
