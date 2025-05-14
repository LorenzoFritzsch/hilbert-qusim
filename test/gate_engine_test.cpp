#include "gate_engine.h"

#include "lazy_matrix_operation_cast.h"
#include "lazy_vector_operation_cast.h"

bool it_should_apply_gate() {
  // Given
  auto gate = std::make_unique<ComplexMatrix>(pauli_x);
  auto gate_lazy = std::make_unique<LazyMatrix>(std::make_unique<LazyMatrixOperationCast>(std::move(gate)));
  auto state = std::make_unique<ComplexVector>(ket_0);
  auto state_lazy = std::make_unique<LazyVector>(std::make_unique<LazyVectorOperationCast>(std::move(state)));

  // When
  const auto result = GateEngine::apply_gate(std::move(gate_lazy), std::move(state_lazy));

  // Then
  const auto expected = ket_1;
  return expected == result->get();
}

bool it_should_apply_controlled_gate() {
  // Given
  const auto control = std::make_unique<Qubit>(0, 1);
  const auto target = std::make_unique<Qubit>(1, 0);
  auto gate = std::make_unique<ComplexMatrix>(pauli_x);

  // When
  const auto result = GateEngine::controlled_u(control, target, std::move(gate));

  // Then
  const auto expected = ket_1;
  return expected == *result->to_vector();
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