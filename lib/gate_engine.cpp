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
#include <stdexcept>
#include <string>

/*
 * Computes the controlled-U matrix given the unitary U.
 */
inline std::unique_ptr<LazyOperation>
make_controlled_u(const ComplexVectMatrix &u) {
  return AlgebraEngine::matsum(
      *AlgebraEngine::tensor_product(*ComplexVectMatrix::proj_k0(),
                                     *LazyOperation::identity(u.row_size())),
      *AlgebraEngine::tensor_product(*ComplexVectMatrix::proj_k1(), u));
}

std::unique_ptr<ComplexVectMatrix>
reduced_density_matrix_control(const LazyOperation &s) {
  if (s.row_size() != 1 || s.column_size() != 4) {
    throw std::invalid_argument(
        "Cannot compute reduced density matrix for state vector of size (" +
        std::to_string(s.row_size()) + "x" + std::to_string(s.column_size()) +
        ") != (1, 4)");
  }
  auto alpha = s.get(0, 0);
  auto beta = s.get(0, 1);
  auto gamma = s.get(0, 2);
  auto delta = s.get(0, 3);
  return std::make_unique<ComplexVectMatrix>(
      ComplexMatrix({{std::norm(alpha) + std::norm(beta),
                      alpha * std::conj(gamma) + beta * std::conj(delta)},
                     {gamma * std::conj(alpha) + delta * std::conj(beta),
                      std::norm(gamma) + std::norm(delta)}}));
}

std::unique_ptr<ComplexVectMatrix>
reduced_density_matrix_target(const LazyOperation &s) {
  if (s.row_size() != 1 || s.column_size() != 4) {
    throw std::invalid_argument(
        "Cannot compute reduced density matrix for state vector of size (" +
        std::to_string(s.row_size()) + "x" + std::to_string(s.column_size()) +
        ") != (1, 4)");
  }
  auto alpha = s.get(0, 0);
  auto beta = s.get(0, 1);
  auto gamma = s.get(0, 2);
  auto delta = s.get(0, 3);
  return std::make_unique<ComplexVectMatrix>(
      ComplexMatrix({{std::norm(alpha) + std::norm(gamma),
                      alpha * std::conj(beta) + gamma * std::conj(delta)},
                     {beta * std::conj(alpha) + delta * std::conj(gamma),
                      std::norm(beta) + std::norm(delta)}}));
}

/*
 * Traces out the qubit from the reduced density matrix.
 */
std::unique_ptr<Qubit> trout(const ComplexVectMatrix &reduced) {
  Complex zero(0), one(1);
  Complex alpha, beta;
  alpha = reduced.get(0, 0) - one;
  beta = reduced.get(0, 1);

  if (approx_equal(alpha, zero) && approx_equal(beta, zero)) {
    alpha = reduced.get(1, 0);
    beta = reduced.get(1, 1) - one;
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
    auto n = static_cast<Complex>(std::sqrt(norm(alpha, beta)));
    alpha = alpha / n;
    beta = beta / n;
  }

  return std::make_unique<Qubit>(alpha, beta);
}

inline void verify_unitarity(const ComplexVectMatrix &u) {
  if (!AlgebraEngine::is_unitary(u)) {
    throw std::invalid_argument("Matrix is not unitary");
  }
}

inline void verify_vector(const LazyOperation &v, const size_t size) {
  if (v.row_size() != 1 || v.column_size() != size) {
    throw std::invalid_argument(
        "LazyOperation does not represent a valid (1, " + std::to_string(size) +
        ") vector: is (" + std::to_string(v.row_size()) + ", " +
        std::to_string(v.column_size()) + ") instead");
  }
}

inline void verify_sqmatrix(const ComplexVectMatrix &m, const size_t dim) {
  if (m.row_size() != m.column_size() || m.row_size() != dim) {
    auto dim_str = std::to_string(dim);
    throw std::invalid_argument("Matrix does not represent a valid (" +
                                dim_str + ", " + dim_str + ") matrix: is (" +
                                std::to_string(m.row_size()) + ", " +
                                std::to_string(m.column_size()) + ") instead");
  }
}

std::unique_ptr<Qubit> GateEngine::trout_control(const LazyOperation &s) {
  verify_vector(s, 4);
  auto reduced = reduced_density_matrix_control(s);
  return trout(*reduced);
}

std::unique_ptr<Qubit> GateEngine::trout_target(const LazyOperation &s) {
  verify_vector(s, 4);
  auto reduced = reduced_density_matrix_target(s);
  return trout(*reduced);
}

std::unique_ptr<LazyOperation>
GateEngine::apply_gate(const ComplexVectMatrix &gate,
                       const ComplexVectMatrix &state) {
  return AlgebraEngine::matrix_vector_product(gate, state);
}

std::unique_ptr<LazyOperation>
GateEngine::controlled_u(const Qubit &control, const StateVector &target,
                         const ComplexVectMatrix &u) {
  // TODO
}

std::unique_ptr<LazyOperation>
GateEngine::controlled_u(std::unique_ptr<LazyOperation> control,
                         std::unique_ptr<LazyOperation> target,
                         const ComplexVectMatrix &u) {
  verify_sqmatrix(u, 2);
  verify_unitarity(u);
  verify_vector(*control, 4);
  verify_vector(*target, 4);
  auto controlled_u = AlgebraEngine::tensor_product(
      *make_controlled_u(u), *LazyOperation::identity(4));
  auto state = AlgebraEngine::tensor_product(*control, *target);
  return AlgebraEngine::matrix_vector_product(*controlled_u, *state);
}

std::unique_ptr<Qubit> GateEngine::controlled_u(const Qubit &target,
                                                const Qubit &control,
                                                const ComplexVectMatrix &u) {
  verify_sqmatrix(u, 2);
  verify_unitarity(u);
  auto controlled_u = make_controlled_u(u);
  auto state =
      AlgebraEngine::tensor_product(*control.to_vector(), *target.to_vector());
  state = AlgebraEngine::matrix_vector_product(*controlled_u, *state);
  return trout_target(*state);
}

std::unique_ptr<LazyOperation>
GateEngine::controlled_u_stv(const Qubit &target, const Qubit &control,
                             const ComplexVectMatrix &u) {
  verify_sqmatrix(u, 2);
  verify_unitarity(u);
  auto controlled_u = make_controlled_u(u);
  const auto state =
      AlgebraEngine::tensor_product(*control.to_vector(), *target.to_vector());
  return AlgebraEngine::matrix_vector_product(*controlled_u, *state);
}

std::unique_ptr<Qubit> GateEngine::hadamard(const Qubit &qubit) {
  auto result = AlgebraEngine::matrix_vector_product(
      *ComplexVectMatrix::hadamard_2x2(), *qubit.to_vector());
  return std::make_unique<Qubit>(*result);
}
