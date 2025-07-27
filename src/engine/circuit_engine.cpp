#include "circuit_engine.h"
#include "gate_engine.h"
#include <memory>

std::vector<Qubit> CircuitEngine::qft(const std::vector<Qubit> j) {
  std::vector<Qubit> result(j.size());

  auto last_index = result.size() - 1;
  for (int i = 0; i < j.size(); i++) {
    auto j_k = GateEngine::hadamard(j[i]);
    for (int k_next = i + 1; k_next < j.size(); k_next++) {
      j_k = GateEngine::controlled_u(*j_k, j[k_next],
                                     *GateEngine::r_k(k_next + 1));
    }
    result[last_index - i] = *j_k;
  }

  return result;
}

std::vector<Qubit> CircuitEngine::inverse_qft(const std::vector<Qubit> k) {
  std::vector<Qubit> result(k.size());

  auto swapped(k);
  std::reverse(swapped.begin(), swapped.end());

  auto last_index = swapped.size() - 1;
  for (int i = last_index; i >= 0; i--) {
    auto k_i = swapped[i];
    for (int j = last_index; j > i; j--) {
      k_i = *GateEngine::controlled_u(k_i, result[j],
                                      *GateEngine::r_k(j + 1, true));
    }
    result[i] = *GateEngine::hadamard(k_i);
  }

  return result;
}
