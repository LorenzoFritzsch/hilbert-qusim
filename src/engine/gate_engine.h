#ifndef GATE_ENGINE_H
#define GATE_ENGINE_H

#include "hilbert_namespace.h"
#include "lazy_matrix.h"

class GateEngine {
public:
  GateEngine() = default;

  static std::shared_ptr<LazyMatrix> apply_gate(std::shared_ptr<LazyMatrix> &state, std::shared_ptr<LazyMatrix> &gate);

  void controlled_u(ComplexVector &control, ComplexVector &target,
                   ComplexMatrix &u);
};

#endif // !GATE_ENGINE_H
