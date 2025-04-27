// TODO: Fix CMake
#include "../src/model/lazy_vector.h"

bool equals(const ComplexVector& lhs, const ComplexVector& rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for (int i = 0; i < lhs.size(); i++) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}

bool it_should_compute_matrix_vector_product() {
  // Given
  auto matrix = std::make_unique<ComplexMatrix>(pauli_x);
  auto vector = std::make_unique<ComplexVector>(ket_0);

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(matrix), std::move(vector));

  // Then
  const auto actual = lazy_v->get();
  const auto expected = ket_1;
  return equals(expected, actual);
}

bool it_should_compute_lazy_matrix_vector_product() {
  // Given
  auto matrix = std::make_unique<LazyMatrix>(LazyMatrix(std::make_unique<ComplexMatrix>(pauli_x), 1));
  auto vector = std::make_unique<ComplexVector>(ket_0);

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(matrix), std::move(vector));

  // Then
  const auto actual = lazy_v->get();
  const auto expected = ket_1;
  return equals(expected, actual);
}

bool it_should_compute_scalar_product() {
  // Given
  auto vector = std::make_unique<ComplexVector>(ket_0);
  constexpr auto k = Complex(1);

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(vector), k);

  // Then
  const auto actual = lazy_v->get();
  const auto expected = ket_0;
  return equals(expected, actual);
}

bool it_should_compute_lazy_matrix_lazy_vector_product() {
  // Given
  auto matrix = std::make_unique<LazyMatrix>(std::make_unique<ComplexMatrix>(pauli_x), 1);
  auto vector = std::make_unique<LazyVector>(std::make_unique<ComplexVector>(ket_0), 1);

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(matrix), std::move(vector));

  // Then
  const auto actual = lazy_v->get();
  const auto expected = ket_1;
  return equals(expected, actual);
}

bool it_should_compute_lazy_scalar_product() {
  // Given
  auto vector = std::make_unique<LazyVector>(std::make_unique<ComplexVector>(ket_0), 1);

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(vector), 1);

  // Then
  const auto actual = lazy_v->get();
  const auto expected = ket_0;
  return equals(expected, actual);
}

int main() {
  int failed = 0;
  if (!it_should_compute_matrix_vector_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_matrix_vector_product()) {
    failed++;
  }
  if (!it_should_compute_scalar_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_matrix_lazy_vector_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_scalar_product()) {
    failed++;
  }
  return failed == 0 ? 0 : 1;
}
