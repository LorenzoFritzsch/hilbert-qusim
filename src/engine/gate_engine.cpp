#include "gate_engine.h"
#include "algebra_engine.h"

std::unique_ptr<LazyOperation> make_controlled_u(std::unique_ptr<OpMember> u) {
  auto proj_i = std::make_unique<ComplexVectMatrix>(
      ComplexMatrix{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}});
  auto ket_1_density_mat =
      std::make_unique<ComplexVectMatrix>(ComplexMatrix{{0, 0}, {0, 1}});
  auto proj_u =
      AlgebraEngine::tensor_product(std::move(ket_1_density_mat), std::move(u));
  return AlgebraEngine::sum(std::move(proj_i), std::move(proj_u));
}

std::unique_ptr<Qubit>
trace_out_target(const std::unique_ptr<LazyOperation> &s) {
  return std::make_unique<Qubit>(
      s->get(0, 0) * s->get(0, 0) + s->get(0, 2) * s->get(0, 2),
      s->get(0, 1) * s->get(0, 1) + s->get(0, 3) * s->get(0, 3));
}

std::unique_ptr<LazyOperation>
GateEngine::apply_gate(std::unique_ptr<OpMember> gate,
                       std::unique_ptr<OpMember> state) {
  return AlgebraEngine::matrix_vector_product(std::move(gate),
                                              std::move(state));
}

std::unique_ptr<Qubit>
GateEngine::controlled_u(const std::unique_ptr<Qubit> &control,
                         const std::unique_ptr<Qubit> &target,
                         std::unique_ptr<OpMember> u) {
  if (!AlgebraEngine::is_unitary(*u)) {
    throw std::runtime_error("U is not unitary");
  }
  auto state = AlgebraEngine::tensor_product(std::move(control->to_vector()),
                                             std::move(target->to_vector()));
  auto transformed_state =
      apply_gate(make_controlled_u(std::move(u)), std::move(state));
  return trace_out_target(transformed_state);
}
