#ifndef GATE_ENGINE_H
#define GATE_ENGINE_H

#include <memory>
#include "lazy_matrix.h"
#include "lazy_vector.h"

class GateEngine {
public:
  GateEngine() = default;

  static std::unique_ptr<LazyVector> apply_gate(const std::unique_ptr<LazyMatrix> &gate,
                                                const std::unique_ptr<LazyVector> &state);

  std::unique_ptr<ComplexVector> controlled_u(std::unique_ptr<ComplexVector> control, std::unique_ptr<ComplexVector> target,
                    std::unique_ptr<ComplexMatrix> u);
};

#endif // !GATE_ENGINE_H
