#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "hilbert_namespace_test.h"
#include "qubit.h"
#include <iostream>
#include <memory>
#include <stdexcept>

bool it_should_create_qubit() {
  // Given
  constexpr auto alpha = Complex(1, 0);
  constexpr auto beta = Complex(0, 0);

  // When
  const auto qubit = std::make_unique<Qubit>(alpha, beta);

  // Then
  return are_matrices_equal(ComplexVectMatrix(ket_0), *qubit->to_vector());
}

bool it_should_not_create_qubit_with_invalid_alpha_beta() {
  // Given
  constexpr auto alpha = Complex(1, 0);
  constexpr auto beta = Complex(1, 0);

  // When - Then
  try {
    const auto qubit = Qubit(alpha, beta);
    return false;
  } catch (const std::invalid_argument &e) {
    return true;
  }
}

bool it_should_create_qubit_opmember() {
  // Given
  auto qubit_vect = std::make_unique<ComplexVectMatrix>(ket_0);

  // When
  const auto qubit = std::make_unique<Qubit>(std::move(qubit_vect));

  // Then
  return are_matrices_equal(ComplexVectMatrix(ket_0), *qubit->to_vector());
}

bool it_should_not_create_qubit_invalid_opmember() {
  // Given
  auto invalid_opmember = std::make_unique<ComplexVectMatrix>(hadamard_2x2);

  // When - Then
  try {
    const auto qubit = std::make_unique<Qubit>(std::move(invalid_opmember));
    return false;
  } catch (const std::invalid_argument &e) {
    return true;
  }
}

bool it_should_not_create_qubit_opmember_invalid_alpha_beta() {
  // Given
  auto qubit_vect = std::make_unique<ComplexVectMatrix>(ComplexVector({1, 1}));

  // When - Then
  try {
    const auto qubit = std::make_unique<Qubit>(std::move(qubit_vect));
    return false;
  } catch (const std::invalid_argument &e) {
    return true;
  }
}

int main() {
  int total = 0;
  int failed = 0;

  if (!it_should_create_qubit()) {
    failed++;
    std::cout << "it_should_create_qubit failed" << std::endl;
  }
  total++;

  if (!it_should_not_create_qubit_with_invalid_alpha_beta()) {
    failed++;
    std::cout << "it_should_not_create_qubit_with_invalid_alpha_beta failed"
              << std::endl;
  }
  total++;

  if (!it_should_create_qubit_opmember()) {
    failed++;
    std::cout << "it_should_create_qubit_opmember failed" << std::endl;
  }
  total++;

  if (!it_should_not_create_qubit_invalid_opmember()) {
    failed++;
    std::cout << "it_should_not_create_qubit_invalid_opmember failed"
              << std::endl;
  }
  total++;

  if (!it_should_not_create_qubit_opmember_invalid_alpha_beta()) {
    failed++;
    std::cout << "it_should_not_create_qubit_opmember_invalid_alpha_beta failed"
              << std::endl;
  }
  total++;

  std::cout << "Run: " << total << ", failed: " << failed << std::endl;
  return failed == 0 ? 0 : 1;
}
