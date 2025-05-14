#ifndef GATE_ENGINE_H
#define GATE_ENGINE_H

#include <memory>
#include "lazy_matrix.h"
#include "lazy_vector.h"
#include "qubit.h"

class GateEngine {
public:
  GateEngine() = default;

  static std::unique_ptr<LazyVector> apply_gate(std::unique_ptr<LazyMatrix> gate,
                                                std::unique_ptr<LazyVector> state);

  static std::unique_ptr<Qubit> controlled_u(const std::unique_ptr<Qubit> &control, const std::unique_ptr<Qubit> &target,
                                             std::unique_ptr<ComplexMatrix> u);
};

#endif // !GATE_ENGINE_H
