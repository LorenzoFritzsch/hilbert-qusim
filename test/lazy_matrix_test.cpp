// TODO: Fix CMake
#include "../src/model/lazy_matrix.h"

#include <iostream>
void printMatrix(const ComplexMatrix &matrix) {
  for (auto & n : matrix) {
    for (auto m : n) {
      std::cout << m << " ";
    }
    std::cout << std::endl;
  }
}

bool areMatricesEqual(const ComplexMatrix &m1, const ComplexMatrix &m2) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[0].size(); j++) {
      if (m1[i][j] != m2[i][j]) {
        return false;
      }
    }
  }
  return true;
}

bool itShouldComputeLazyMatrix_4x4_tensorialProduct() {
  // Given
  const auto a = hadamard_2x2;
  const auto b = identity_2x2;

  // When
  const auto cLazy = new LazyMatrix(a, b, LazyMatrixOperation::TENSORIAL_PRODUCT);

  // Then
  const ComplexMatrix expected = {
    {1 / std::sqrt(2), 0, 1 / std::sqrt(2), 0},
    {0, 1 / std::sqrt(2), 0, 1 / std::sqrt(2)},
    {1 / std::sqrt(2), 0, -1 / std::sqrt(2), -0},
    {0, 1 / std::sqrt(2), -0, -1 / std::sqrt(2)},
  };

  return expected == cLazy->get();
}

bool itShouldComputeLazyMatrix_8x8_tensorialProduct() {
  // Given
  const ComplexMatrix m = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };

  // When
  const auto cLazy = new LazyMatrix(m, m, LazyMatrixOperation::TENSORIAL_PRODUCT);

  // Then
  const ComplexMatrix expected = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
  };
  return areMatricesEqual(expected, cLazy->get());
}

bool itShouldComputeLazyMatrix_2x2_scalarProduct() {
  // Given
  const auto m = identity_2x2;
  constexpr Complex k = {1, 4};

  // When
  const auto cLazy = new LazyMatrix(m, k);

  // Then
  const ComplexMatrix expected = {
    {k, 0},
    {0, k}
  };
  return areMatricesEqual(expected, cLazy->get());
}

int main() {
  int failed = 0;
  if (!itShouldComputeLazyMatrix_4x4_tensorialProduct()) {
    failed++;
  }
  if (!itShouldComputeLazyMatrix_8x8_tensorialProduct()) {
    failed++;
  }
  if (!itShouldComputeLazyMatrix_2x2_scalarProduct()) {
    failed++;
  }
  return failed == 0 ? 0 : 1;
}