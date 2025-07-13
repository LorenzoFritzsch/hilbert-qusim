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

  static std::unique_ptr<Qubit>
  controlled_u(const std::unique_ptr<Qubit> &control,
               const std::unique_ptr<Qubit> &target,
               std::unique_ptr<OpMember> u);
};

#endif // !GATE_ENGINE_H
