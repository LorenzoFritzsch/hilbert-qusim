#ifndef GATE_ENGINE_H
#define GATE_ENGINE_H

#include "hilbert_namespace.h"

class GateEngine {
public:
  GateEngine() = default;

  void hadamardGate(ComplexMatrix &state);

  void controlledU(ComplexVector &control, ComplexVector &target,
                   ComplexMatrix &u);
};

#endif // !GATE_ENGINE_H
