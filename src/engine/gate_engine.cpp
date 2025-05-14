#include "gate_engine.h"

#include "algebra_engine.h"
#include "lazy_matrix_operation_cast.h"
#include "lazy_matrix_operation_lazy_sum.h"
#include "lazy_matrix_operation_tensor_product.h"
#include "lazy_vector_operation_lazy_matrix_vector_product.h"
#include "lazy_vector_operation_tensor_product.h"

std::unique_ptr<LazyMatrix> make_controlled_u(std::unique_ptr<ComplexMatrix> u) {
  auto a = std::make_unique<ComplexMatrix>(ComplexMatrix{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  });
  auto b = std::make_unique<ComplexMatrix>(ComplexMatrix{
    {0, 0},
    {0, 1}
  });
  auto a_lazy = std::make_unique<LazyMatrix>(std::make_unique<LazyMatrixOperationCast>(std::move(a)));
  auto b_lazy = std::make_unique<LazyMatrix>(
    std::make_unique<LazyMatrixOperationTensorProduct>(std::move(b), std::move(u)));
  return std::make_unique<LazyMatrix>(
    std::make_unique<LazyMatrixOperationLazySum>(std::move(a_lazy), std::move(b_lazy)));
}

std::unique_ptr<Qubit> trace_out_target(std::unique_ptr<LazyVector> &s) {
  return std::make_unique<Qubit>(s->get(0) * s->get(0) + s->get(2) * s->get(2),
                                 s->get(1) * s->get(1) + s->get(3) * s->get(3));
}

std::unique_ptr<LazyVector>
GateEngine::apply_gate(std::unique_ptr<LazyMatrix> gate, std::unique_ptr<LazyVector> state) {
  return std::make_unique<LazyVector>(
    std::make_unique<LazyVectorOperationLazyMatrixVectorProduct>(std::move(gate), std::move(state)));
}

std::unique_ptr<Qubit> GateEngine::controlled_u(const std::unique_ptr<Qubit> &control,
                                                const std::unique_ptr<Qubit> &target,
                                                std::unique_ptr<ComplexMatrix> u) {
  if (!AlgebraEngine::is_unitary(*u)) {
    throw std::runtime_error("U is not unitary");
  }
  auto state = std::make_unique<LazyVector>(
    std::make_unique<LazyVectorOperationTensorProduct>(std::move(control->to_vector()),
                                                       std::move(target->to_vector())));
  auto transformed_state = apply_gate(std::move(make_controlled_u(std::move(u))), std::move(state));
  return trace_out_target(transformed_state);
}
