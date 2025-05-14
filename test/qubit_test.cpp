#include "qubit.h"

bool it_should_create_qubit() {
  // Given
  constexpr auto alpha = Complex(1, 0);
  constexpr auto beta = Complex(0, 0);

  // When
  const auto qubit = Qubit(alpha, beta);

  // Then
  const auto expected = ComplexVector({alpha, beta});
  return expected == *qubit.to_vector();
}

bool it_should_not_create_qubit_with_invalid_alpha_beta() {
  // Given
  constexpr auto alpha = Complex(1, 0);
  constexpr auto beta = Complex(1, 0);

  // When - Then
  try {
    const auto qubit = Qubit(alpha, beta);
    return false;
  } catch (const std::invalid_argument& e) {
    return true;
  }
}

int main() {
  int failed = 0;
  if (!it_should_create_qubit()) {
    failed++;
  }
  if (!it_should_not_create_qubit_with_invalid_alpha_beta()) {
    failed++;
  }
  return failed == 0 ? 0 : 1;
}