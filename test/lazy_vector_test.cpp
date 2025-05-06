#include "lazy_vector.h"
#include "lazy_matrix.h"
#include "lazy_matrix_operation_cast.h"
#include "lazy_vector_operation_cast.h"
#include "lazy_vector_operation_lazy_matrix_vector_product.h"
#include "lazy_vector_operation_lazy_scalar_product.h"
#include "lazy_vector_operation_lazy_tensor_product.h"
#include "lazy_vector_operation_matrix_vector_product.h"
#include "lazy_vector_operation_scalar_product.h"
#include "lazy_vector_operation_tensor_product.h"

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

std::unique_ptr<LazyVector> to_lazy(std::unique_ptr<ComplexVector> v) {
  auto operation = std::make_unique<LazyVectorOperationCast>(std::move(v));
  return std::make_unique<LazyVector>(std::move(operation));
}

bool it_should_compute_matrix_vector_product() {
  // Given
  auto matrix = std::make_unique<ComplexMatrix>(pauli_x);
  auto vector = std::make_unique<ComplexVector>(ket_0);
  auto operation = std::make_unique<LazyVectorOperationMatrixVectorProduct>(std::move(matrix), std::move(vector));

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(operation));

  // Then
  const auto actual = lazy_v->get();
  const auto expected = ket_1;
  return equals(expected, actual);
}

bool it_should_compute_lazy_matrix_vector_product() {
  // Given
  auto m = std::make_unique<ComplexMatrix>(pauli_x);
  auto operation_m = std::make_unique<LazyMatrixOperationCast>(std::move(m));
  auto matrix = std::make_unique<LazyMatrix>(std::move(operation_m));
  auto vector = to_lazy(std::make_unique<ComplexVector>(ket_0));
  auto operation = std::make_unique<LazyVectorOperationLazyMatrixVectorProduct>(std::move(matrix), std::move(vector));

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(operation));

  // Then
  const auto actual = lazy_v->get();
  const auto expected = ket_1;
  return equals(expected, actual);
}

bool it_should_compute_scalar_product() {
  // Given
  auto vector = std::make_unique<ComplexVector>(ket_0);
  auto k = Complex(1);
  auto operation = std::make_unique<LazyVectorOperationScalarProduct>(std::move(vector), std::make_unique<Complex>(k));

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(operation));

  // Then
  const auto actual = lazy_v->get();
  const auto expected = ket_0;
  return equals(expected, actual);
}

bool it_should_compute_lazy_scalar_product() {
  // Given
  auto vector = to_lazy(std::make_unique<ComplexVector>(ket_0));
  auto k = Complex(3, 8);
  auto operation = std::make_unique<LazyVectorOperationLazyScalarProduct>(
    std::move(vector), std::make_unique<Complex>(k));

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(operation));

  // Then
  const auto actual = lazy_v->get();
  const ComplexVector expected = {k, 0};
  return equals(expected, actual);
}

bool it_should_compute_tensor_product() {
  // Given
  auto a = std::make_unique<ComplexVector>(ket_0);
  auto b = std::make_unique<ComplexVector>(ket_1);
  auto operation = std::make_unique<LazyVectorOperationTensorProduct>(std::move(a), std::move(b));

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(operation));

  // Then
  const auto actual = lazy_v->get();
  const ComplexVector expected = {0, 1, 0, 0};
  return equals(expected, actual);
}

bool it_should_compute_lazy_tensor_product() {
  // Given
  auto a = to_lazy(std::make_unique<ComplexVector>(ket_0));
  auto b = to_lazy(std::make_unique<ComplexVector>(ket_1));
  auto operation = std::make_unique<LazyVectorOperationLazyTensorProduct>(std::move(a), std::move(b));

  // When
  const auto lazy_v = std::make_unique<LazyVector>(std::move(operation));

  // Then
  const auto actual = lazy_v->get();
  const ComplexVector expected = {0, 1, 0, 0};
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
  if (!it_should_compute_lazy_scalar_product()) {
    failed++;
  }
  if (!it_should_compute_tensor_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_tensor_product()) {
    failed++;
  }
  return failed == 0 ? 0 : 1;
}
