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
  if (m1.size() != m2.size()) {
    return false;
  }
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
  const auto a = std::make_shared<ComplexMatrix>(hadamard_2x2);
  const auto b = std::make_shared<ComplexMatrix>(identity_2x2);

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

bool itShouldComputeLazyMatrix_16x16_tensorialProduct() {
  // Given
  const auto m = std::make_shared<ComplexMatrix>(ComplexMatrix{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  });

  // When
  const auto cLazy = new LazyMatrix(m, m, LazyMatrixOperation::TENSORIAL_PRODUCT);

  // Then
  const ComplexMatrix expected = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
  };
  return areMatricesEqual(expected, cLazy->get());
}

bool itShouldComputeLazyMatrix_8x8_fromLazyMatrices() {
  // Given
  const auto a = std::make_shared<ComplexMatrix>(identity_2x2);
  const auto aLazy = std::make_shared<LazyMatrix>(a, a, LazyMatrixOperation::TENSORIAL_PRODUCT);
  const auto bLazy = std::make_shared<LazyMatrix>(a, 1);

  // When
  const auto cLazy = new LazyMatrix(aLazy, bLazy, LazyMatrixOperation::TENSORIAL_PRODUCT);

  // Then
  const ComplexMatrix expected = {
    {1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 1}
  };
  return areMatricesEqual(expected, cLazy->get());
}

bool itShouldComputeLazyMatrix_2x2_scalarProduct() {
  // Given
  const auto m = std::make_shared<ComplexMatrix>(identity_2x2);
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
  if (!itShouldComputeLazyMatrix_16x16_tensorialProduct()) {
    failed++;
  }
  if (!itShouldComputeLazyMatrix_2x2_scalarProduct()) {
    failed++;
  }
  if (!itShouldComputeLazyMatrix_8x8_fromLazyMatrices()) {
    failed++;
  }
  return failed == 0 ? 0 : 1;
}