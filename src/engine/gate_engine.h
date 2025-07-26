#ifndef GATE_ENGINE_H
#define GATE_ENGINE_H

#include "lazy_operation.h"
#include "qubit.h"
#include <memory>

class GateEngine {
public:
  GateEngine() = delete;

  static std::unique_ptr<LazyOperation>
  apply_gate(std::unique_ptr<OpMember> gate, std::unique_ptr<OpMember> state);

  static std::unique_ptr<Qubit> controlled_u(const Qubit &target,
                                             const Qubit &control,
                                             std::unique_ptr<OpMember> u);

  static std::unique_ptr<Qubit> hadamard(const Qubit &qubit);

  static std::unique_ptr<ComplexVectMatrix> r_k(const int k,
                                                bool inverse = false);
};

#endif // !GATE_ENGINE_H
