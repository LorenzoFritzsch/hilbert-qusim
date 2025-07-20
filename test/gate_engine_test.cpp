#include "complex_vectorised_matrix.h"
#include "gate_engine.h"
#include "hilbert_namespace_test.h"
#include <cmath>
#include <iostream>
#include <memory>

bool it_should_apply_gate() {
  // Given
  auto gate = std::make_unique<ComplexVectMatrix>(pauli_x);
  auto state = std::make_unique<ComplexVectMatrix>(ket_0);

  // When
  const auto result = GateEngine::apply_gate(std::move(gate), std::move(state));

  // Then
  return are_matrices_equal(ComplexVectMatrix(ket_1), *result);
}

bool it_should_apply_controlled_gate() {
  // Given
  auto target = std::make_unique<Qubit>(1, 0);
  auto control = std::make_unique<Qubit>(0, 1);
  auto gate = std::make_unique<ComplexVectMatrix>(pauli_x);

  // When
  auto result = GateEngine::controlled_u(*target, *control, std::move(gate));

  // Then
  const auto expected = ket_1;
  return are_matrices_equal(ComplexVectMatrix(ket_1), *result->to_vector());
}

bool it_should_apply_hadamard() {
  // Given
  auto qubit = std::make_unique<Qubit>(1, 0);

  // When
  auto result = GateEngine::hadamard(*qubit);

  // Then
  auto alpha_beta_exp = 1 / std::sqrt(2);
  const auto expected = std::make_unique<Qubit>(alpha_beta_exp, alpha_beta_exp);
  return are_matrices_equal(*expected->to_vector(), *result->to_vector());
}

int main() {
  int total = 0;
  int failed = 0;

  if (!it_should_apply_gate()) {
    failed++;
    std::cout << "it_should_apply_gate failed" << std::endl;
  }
  total++;

  if (!it_should_apply_controlled_gate()) {
    failed++;
    std::cout << "it_should_apply_controlled_gate failed" << std::endl;
  }
  total++;

  if (!it_should_apply_hadamard()) {
    failed++;
    std::cout << "it_should_apply_hadamard failed" << std::endl;
  }
  total++;

  std::cout << "Run: " << total << ", failed: " << failed << std::endl;
  return failed == 0 ? 0 : 1;
}
