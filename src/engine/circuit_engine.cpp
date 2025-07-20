#include "circuit_engine.h"
#include "gate_engine.h"
#include <memory>

std::vector<Qubit> CircuitEngine::qft(const std::vector<Qubit> j) {
  std::vector<Qubit> result(j.size());

  for (int i = 0; i < j.size(); i++) {
    auto j_k = GateEngine::hadamard(j[i]);
    for (int k_next = i + 1; k_next < j.size(); k_next++) {
      j_k = GateEngine::controlled_u(*j_k, j[k_next],
                                     GateEngine::r_k(k_next + 1));
    }
    result[i] = *j_k;
  }

  std::reverse(result.begin(), result.end());
  return result;
}
