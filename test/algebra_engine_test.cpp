#include "algebra_engine.h"
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "hilbert_namespace_test.h"
#include <initializer_list>
#include <memory>

#define PERFORMANCE_TESTING 0

#if PERFORMANCE_TESTING
#include <chrono>
#include <iostream>

std::string format_with_dots(const unsigned long long number) {
  std::string num = std::to_string(number);
  int insertPosition = static_cast<int>(num.length()) - 3;

  while (insertPosition > 0) {
    num.insert(insertPosition, ".");
    insertPosition -= 3;
  }

  return num;
}
#endif

bool it_should_compute_conjugate_transpose() {
  // Given
  ComplexMatrix complex_mat = {{{1, 1}, {1, 2}}, {{2, 1}, {2, 2}}};

  // When
  auto result = AlgebraEngine::conjugate_transpose(complex_mat);

  // Then
  ComplexMatrix expected = {{{1, -1}, {2, -1}}, {{1, -2}, {2, -2}}};
  return are_matrices_equal(ComplexVectMatrix(ComplexMatrix(expected)),
                            *result);
}

bool it_should_compute_inner_product() {
  // Given
  auto complex_vector_a =
      std::make_unique<ComplexVectMatrix>(ComplexVector(1, 1));
  auto complex_vector_b =
      std::make_unique<ComplexVectMatrix>(ComplexVector(1, 1));
  auto complex_vector_c =
      std::make_unique<ComplexVectMatrix>(ComplexVector(1, 1));
  auto complex_vector_d =
      std::make_unique<ComplexVectMatrix>(ComplexVector(1, 0));

  // When
  const auto result_a_b =
      AlgebraEngine::inner_product(*complex_vector_a, *complex_vector_b);
  const auto result_a_c =
      AlgebraEngine::inner_product(*complex_vector_c, *complex_vector_d);

  // Then
  const bool is_inner_product_between_equal_vectors_one =
      are_matrices_equal(ComplexVectMatrix(Complex(1, 0)), *result_a_b);
  const bool is_inner_product_between_orthogonal_vectors_zero =
      are_matrices_equal(ComplexVectMatrix(Complex(0, 0)), *result_a_c);
  return is_inner_product_between_equal_vectors_one &&
         is_inner_product_between_orthogonal_vectors_zero;
}

bool it_should_compute_outer_product() {
  // Given
  auto a = std::make_unique<ComplexVectMatrix>(ComplexVector(ket_0));
  auto b = std::make_unique<ComplexVectMatrix>(ComplexVector(ket_1));

  // When
  const auto result = AlgebraEngine::outer_product(*a, *b);

  // Then
  const ComplexMatrix expected = {{0, 1}, {0, 0}};

  return are_matrices_equal(ComplexVectMatrix(ComplexMatrix(expected)),
                            *result);
}

bool it_should_compute_matrix_vector_product() {
  // Given
  auto mat = std::make_unique<ComplexVectMatrix>(ComplexMatrix(pauli_x));
  auto vect = std::make_unique<ComplexVectMatrix>(ComplexVector(ket_0));

  // When
  const auto result = AlgebraEngine::matrix_vector_product(*mat, *vect);

  // Then
  const auto expected =
      std::make_unique<ComplexVectMatrix>(ComplexVector(ket_1));
  return are_matrices_equal(*expected, *result);
}

bool it_should_compute_scalar_product() {
  // Given
  auto a = std::make_unique<ComplexVectMatrix>(ComplexMatrix(identity_2x2));
  constexpr Complex k = {1, 4};

  // When
  const auto result = AlgebraEngine::scalar_product(*a, Complex(k));

  // Then
  const ComplexMatrix expected = {{k, 0}, {0, k}};
  return are_matrices_equal(ComplexVectMatrix(ComplexMatrix(expected)),
                            *result);
}

bool it_should_compute_scalar_vector_product() {
  // Given
  auto complex_vector = std::make_unique<ComplexVectMatrix>(
      ComplexVector(std::initializer_list<Complex>{{1, 1}, {2, 1}}));

  // When
  const auto result =
      AlgebraEngine::scalar_product(*complex_vector, Complex(2, 2));

  // Then
  const ComplexVector expected = {{0, 4}, {2, 6}};
  return are_matrices_equal(ComplexVectMatrix(ComplexVector(expected)),
                            *result);
}

bool it_should_compute_sum() {
  // Given
  auto a = std::make_unique<ComplexVectMatrix>(ComplexMatrix(identity_2x2));
  auto b = std::make_unique<ComplexVectMatrix>(ComplexMatrix(identity_2x2));

  // When
  const auto result = AlgebraEngine::sum(*a, *b);

  // Then
  const ComplexMatrix expected = {{2, 0}, {0, 2}};
  return are_matrices_equal(ComplexVectMatrix(ComplexMatrix(expected)),
                            *result);
}

bool it_should_compute_tensor_product() {
  // Given
  auto a = std::make_unique<ComplexVectMatrix>(ComplexMatrix(hadamard_2x2));
  auto b = std::make_unique<ComplexVectMatrix>(ComplexMatrix(identity_2x2));

  // When
  const auto result = AlgebraEngine::tensor_product(*a, *b);

  // Then
  const ComplexMatrix expected = {
      {1 / std::sqrt(2), 0, 1 / std::sqrt(2), 0},
      {0, 1 / std::sqrt(2), 0, 1 / std::sqrt(2)},
      {1 / std::sqrt(2), 0, -1 / std::sqrt(2), -0},
      {0, 1 / std::sqrt(2), -0, -1 / std::sqrt(2)},
  };

  return are_matrices_equal(ComplexVectMatrix(ComplexMatrix(expected)),
                            *result);
}

bool it_should_compute_matrix_power() {
  // Given
  auto a = std::make_unique<ComplexVectMatrix>(ComplexMatrix(identity_2x2));
  constexpr auto times = 8;

  // When
#if PERFORMANCE_TESTING
  const auto start_lazy = std::chrono::high_resolution_clock::now();
#endif

  const auto result = AlgebraEngine::tensor_product(*a, times);

#if PERFORMANCE_TESTING
  const auto end_lazy = std::chrono::high_resolution_clock::now();
  const auto duration_lazy =
      std::chrono::duration_cast<std::chrono::microseconds>(end_lazy -
                                                            start_lazy);
  std::cout << "** Elapsed time " << times
            << " lazy tensor products: " << duration_lazy.count() << " micros"
            << std::endl;
#endif

  // Then
#if PERFORMANCE_TESTING
  const auto start = std::chrono::high_resolution_clock::now();
#endif
  const auto actual = result->to_matrix();
#if PERFORMANCE_TESTING
  const auto end = std::chrono::high_resolution_clock::now();
  const auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << " - Elapsed time materialisation: "
            << format_with_dots(duration.count()) << " millis" << std::endl;
  std::cout << " - Final matrix: " << actual->row_size() << "x"
            << actual->column_size() << std::endl;
  std::cout << " - Total elements: "
            << format_with_dots(actual->row_size() * actual->column_size())
            << std::endl;
#endif
  return verify_identity_matrix(*actual, std::pow(2, times));
}

bool it_should_verify_unitarity() {
  // Given
  auto a = std::make_unique<ComplexVectMatrix>(ComplexMatrix(hadamard_2x2));

  // When
#if PERFORMANCE_TESTING
  const auto start = std::chrono::high_resolution_clock::now();
#endif

  const auto result = AlgebraEngine::is_unitary(*a);

#if PERFORMANCE_TESTING
  const auto end = std::chrono::high_resolution_clock::now();
  const auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "** Elapsed time unitarity check: "
            << format_with_dots(duration.count()) << " microseconds"
            << std::endl;
#endif

  // Then
  return result;
}

int main() {
  int total = 0;
  int failed = 0;

#if !PERFORMANCE_TESTING
  run_test("it_should_compute_conjugate_transpose",
           it_should_compute_conjugate_transpose, failed, total);

  run_test("it_should_compute_inner_product", it_should_compute_inner_product,
           failed, total);

  run_test("it_should_compute_tensor_product", it_should_compute_tensor_product,
           failed, total);

  run_test("it_should_compute_scalar_product", it_should_compute_scalar_product,
           failed, total);

  run_test("it_should_compute_scalar_vector_product",
           it_should_compute_scalar_vector_product, failed, total);

  run_test("it_should_compute_sum", it_should_compute_sum, failed, total);

  run_test("it_should_compute_outer_product", it_should_compute_outer_product,
           failed, total);

  run_test("it_should_compute_matrix_vector_product",
           it_should_compute_matrix_vector_product, failed, total);

#endif
  run_test("it_should_verify_unitarity", it_should_verify_unitarity, failed,
           total);

  run_test("it_should_compute_matrix_power", it_should_compute_matrix_power,
           failed, total);

  test_resumen(failed, total);
  return failed == 0 ? 0 : 1;
}
