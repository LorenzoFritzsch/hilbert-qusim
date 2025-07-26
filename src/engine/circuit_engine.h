#ifndef CIRCUIT_ENGINE_H
#define CIRCUIT_ENGINE_H

#include "qubit.h"
#include <vector>

class CircuitEngine {
public:
  CircuitEngine() = delete;

  static std::vector<Qubit> qft(const std::vector<Qubit> j);

  static std::vector<Qubit> inverse_qft(const std::vector<Qubit> k);
};

#endif // !CIRCUIT_ENGINE_H
