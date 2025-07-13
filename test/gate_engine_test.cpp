#include "complex_vectorised_matrix.h"
#include "gate_engine.h"
#include "hilbert_namespace_test.h"
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
  auto control = std::make_unique<Qubit>(0, 1);
  auto target = std::make_unique<Qubit>(1, 0);
  auto gate = std::make_unique<ComplexVectMatrix>(pauli_x);

  // When
  auto result = GateEngine::controlled_u(std::move(control), std::move(target),
                                         std::move(gate));

  // Then
  const auto expected = ket_1;
  return are_matrices_equal(ComplexVectMatrix(ket_1), *result->to_vector());
}

int main() {
  int failed = 0;
  if (!it_should_apply_gate()) {
    failed++;
  }
  if (!it_should_apply_controlled_gate()) {
    failed++;
  }
  return failed == 0 ? 0 : 1;
}
