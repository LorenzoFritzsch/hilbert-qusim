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

#include "gate_engine.h"
#include "algebra_engine.h"
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include "qubit.h"
#include <complex>
#include <memory>
#include <numbers>
#include <stdexcept>

std::unique_ptr<LazyOperation> make_controlled_u(const ComplexVectMatrix &u) {
  auto proj_i = ComplexVectMatrix(
      ComplexMatrix{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}});
  auto ket_1_density_mat = ComplexVectMatrix(ComplexMatrix{{0, 0}, {0, 1}});
  auto proj_u = AlgebraEngine::tensor_product(ket_1_density_mat, u);
  return AlgebraEngine::sum(proj_i, *proj_u);
}

std::unique_ptr<ComplexVectMatrix>
reduced_density_matrix(const LazyOperation &s) {
  if (s.row_size() != 1 || s.column_size() != 4) {
    throw std::invalid_argument(
        "Cannot compute reduced density matrix for state vector of size != 4");
  }
  auto alpha = s.get(0, 0);
  auto beta = s.get(0, 1);
  auto gamma = s.get(0, 2);
  auto delta = s.get(0, 3);
  return std::make_unique<ComplexVectMatrix>(
      ComplexMatrix({{alpha * std::conj(alpha) + gamma * std::conj(gamma),
                      alpha * std::conj(beta) + gamma * std::conj(delta)},
                     {beta * std::conj(alpha) + delta * std::conj(gamma),
                      beta * std::conj(beta) + delta * std::conj(delta)}}));
}

std::unique_ptr<Qubit> trace_out_target(const LazyOperation &s) {
  auto reduced = reduced_density_matrix(s);
  Complex zero(0), one(1);
  Complex alpha, beta;
  alpha = reduced->get(0, 0) - one;
  beta = reduced->get(0, 1);

  if (approx_equal(alpha, zero) && approx_equal(beta, zero)) {
    alpha = reduced->get(1, 0);
    beta = reduced->get(1, 1) - one;
  }

  if (approx_equal(alpha, zero)) {
    alpha = one;
    beta = zero;
  } else if (approx_equal(beta, zero)) {
    alpha = zero;
    beta = one;
  } else {
    beta = (-alpha) / beta;
    alpha = one;
    auto norm =
        static_cast<Complex>(std::sqrt(std::norm(alpha) + std::norm(beta)));
    alpha = alpha / norm;
    beta = beta / norm;
  }

  return std::make_unique<Qubit>(alpha, beta);
}

std::unique_ptr<LazyOperation>
GateEngine::apply_gate(const ComplexVectMatrix &gate,
                       const ComplexVectMatrix &state) {
  return AlgebraEngine::matrix_vector_product(gate, state);
}

std::unique_ptr<Qubit> GateEngine::controlled_u(const Qubit &target,
                                                const Qubit &control,
                                                const ComplexVectMatrix &u) {
  if (!AlgebraEngine::is_unitary(u)) {
    throw std::runtime_error("U is not unitary");
  }
  auto state =
      AlgebraEngine::tensor_product(*control.to_vector(), *target.to_vector());
  auto transformed_state = AlgebraEngine::matrix_vector_product(
      std::move(make_controlled_u(u)), std::move(state));
  return trace_out_target(*transformed_state);
}

std::unique_ptr<Qubit> GateEngine::hadamard(const Qubit &qubit) {
  auto result = AlgebraEngine::matrix_vector_product(
      ComplexVectMatrix(hadamard_2x2), *qubit.to_vector());
  return std::make_unique<Qubit>(*result);
}

std::unique_ptr<ComplexVectMatrix> GateEngine::r_k(const int k, bool inverse) {
  auto rotation = 2 * std::numbers::pi / std::pow(2, k);
  if (inverse) {
    rotation = -rotation;
  }
  auto rotation_real = std::cos(rotation);
  auto rotation_imag = std::sin(rotation);
  return std::make_unique<ComplexVectMatrix>(
      ComplexMatrix({{1, 0}, {0, Complex(rotation_real, rotation_imag)}}));
}
