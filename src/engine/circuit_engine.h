#ifndef CIRCUIT_ENGINE_H
#define CIRCUIT_ENGINE_H

#include "qubit.h"
#include <vector>

class CircuitEngine {
public:
  CircuitEngine() = delete;

  static std::vector<Qubit> qft(const std::vector<Qubit> j);
};

#endif // !CIRCUIT_ENGINE_H
