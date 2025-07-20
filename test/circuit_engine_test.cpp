#include "circuit_engine.h"
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace_test.h"
#include "qubit.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

void print_states(const std::vector<Qubit> &state,
                  const std::vector<Qubit> &result) {
  for (int i = 0; i < state.size(); i++) {
    auto left = state[i].to_vector();
    auto right = result[i].to_vector();
    std::cout << "(" << left->get(0, 0).real() << ", " << left->get(0, 0).imag()
              << ")|0> + (" << left->get(0, 1).real() << ", "
              << left->get(0, 1).imag() << ")|1> => ("
              << right->get(0, 0).real() << ", " << right->get(0, 0).imag()
              << ")|0> + (" << right->get(0, 1).real() << ", "
              << right->get(0, 1).imag() << ")|1>" << std::endl;
  }
}

bool are_states_equal(const std::vector<Qubit> &left,
                      const std::vector<Qubit> &right) {
  if (left.size() != right.size()) {
    return false;
  }
  for (int i = 0; i < left.size(); i++) {
    if (left[i] != right[i]) {
      return false;
    }
  }
  return true;
}

bool it_should_compute_qft() {
  // Given
  auto ket_0_q = Qubit(1, 0);
  auto ket_1_q = Qubit(0, 1);
  auto state_1 = std::vector<Qubit>({ket_0_q, ket_0_q});
  auto state_2 = std::vector<Qubit>({ket_0_q, ket_1_q});

  // When
  auto result_1 = CircuitEngine::qft(state_1);
  auto result_2 = CircuitEngine::qft(state_2);

  // Then
  auto ket_a_1 = Qubit(1 / std::sqrt(2), 1 / std::sqrt(2));
  auto expected_1 = std::vector<Qubit>({ket_a_1, ket_a_1});
  bool case_1_ok = are_states_equal(expected_1, result_1);

  auto ket_a_2 = Qubit(1 / std::sqrt(2), -1 / std::sqrt(2));
  auto ket_b_2 = Qubit(1 / std::sqrt(2), Complex(0, 1 / std::sqrt(2)));
  auto expected_2 = std::vector<Qubit>({ket_a_2, ket_b_2});
  bool case_2_ok = are_states_equal(expected_2, result_2);

  return case_1_ok && case_2_ok;
}

int main() {
  int total = 0;
  int failed = 0;

  if (!it_should_compute_qft()) {
    failed++;
    std::cout << "it_should_compute_qft failed" << std::endl;
  }
  total++;

  std::cout << "Run: " << total << ", failed: " << failed << std::endl;
  return failed == 0 ? 0 : 1;
}
