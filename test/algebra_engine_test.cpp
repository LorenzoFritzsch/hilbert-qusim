#include "algebra_engine.h"

bool verify_identity_matrix(const ComplexMatrix &matrix) {
  if (matrix.size() != matrix[0].size()) {
    return false;
  }
  for (auto n = 0; n < matrix.size(); n++) {
    for (auto m = 0; m < matrix.size(); m++) {
      if (n == m) {
        if (matrix[n][m] != Complex(1, 0)) {
          return false;
        }
      } else {
        if (matrix[n][m] != Complex(0, 0)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool it_should_compute_conjugate_transpose() {
  // Given
  auto complex_matrix = ComplexMatrix(2, ComplexVector(2, 1));
  complex_matrix[0][0] = Complex(1, 1);
  complex_matrix[0][1] = Complex(1, 2);
  complex_matrix[1][0] = Complex(2, 1);
  complex_matrix[1][1] = Complex(2, 2);

  // When
  auto conjugate_transpose = AlgebraEngine::conjugate_transpose(complex_matrix);
  conjugate_transpose = AlgebraEngine::conjugate_transpose(conjugate_transpose);

  // Then - Applying the conjugate transpose twice should return the original matrix
  return complex_matrix == conjugate_transpose;
}

bool it_should_multiply_vector_by_scalar() {
  // Given
  const auto complex_vector = ComplexVector(2, 1);
  constexpr auto scalar = Complex(2, 2);

  // When
  const auto result = AlgebraEngine::multiply(complex_vector, scalar);

  // Then
  return result[0] == Complex(2, 2) && result[1] == Complex(2, 2);
}

bool it_should_compute_inner_product_between_two_vectors() {
  // Given
  const auto complex_vector_a = ComplexVector(1, 1);
  const auto complex_vector_b = ComplexVector(1, 1);
  const auto complex_vector_c = ComplexVector(1, 0);

  // When
  const auto result_a_scalar_b = AlgebraEngine::inner_product(complex_vector_a, complex_vector_b);
  const auto result_a_scalar_c = AlgebraEngine::inner_product(complex_vector_a, complex_vector_c);

  // Then
  const bool is_scalar_between_orthogonal_vectors_zero = result_a_scalar_c == Complex(0, 0);
  const bool is_scalar_between_equal_vectors_one = result_a_scalar_b == Complex(1, 0);
  return is_scalar_between_orthogonal_vectors_zero && is_scalar_between_equal_vectors_one;
}

bool it_should_compute_matrix_power() {
  // Given
  auto a = std::make_unique<ComplexMatrix>(identity_2x2);
  constexpr auto times = 8;

  // When
  const auto result = AlgebraEngine::tensorial_product(std::move(a), times);

  // Then
  const auto actual = result->get();
  return verify_identity_matrix(actual)
         && static_cast<int>(actual.size()) == std::pow(2, times);
}

int main() {
  int failed = 0;

  if (!it_should_compute_conjugate_transpose()) {
    failed += 1;
  }
  if (!it_should_multiply_vector_by_scalar()) {
    failed += 1;
  }
  if (!it_should_compute_inner_product_between_two_vectors()) {
    failed += 1;
  }
  if (!it_should_compute_matrix_power()) {
    failed += 1;
  }

  return failed == 0 ? 0 : 1;
}
