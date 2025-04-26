// TODO: Fix CMake
#include "../src/model/lazy_matrix.h"

#include <iostream>
void print_matrix(const ComplexMatrix &matrix) {
  for (auto & n : matrix) {
    for (auto m : n) {
      std::cout << m << " ";
    }
    std::cout << std::endl;
  }
}

bool equals(const ComplexMatrix &m1, const ComplexMatrix &m2) {
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

bool it_should_compute_lazy_matrix_tensorial_product() {
  // Given
  const auto a = std::make_shared<ComplexMatrix>(hadamard_2x2);
  const auto b = std::make_shared<ComplexMatrix>(identity_2x2);

  // When
  const auto c_lazy = new LazyMatrix(a, b, LazyMatrixOperation::TENSORIAL_PRODUCT);

  // Then
  const ComplexMatrix expected = {
    {1 / std::sqrt(2), 0, 1 / std::sqrt(2), 0},
    {0, 1 / std::sqrt(2), 0, 1 / std::sqrt(2)},
    {1 / std::sqrt(2), 0, -1 / std::sqrt(2), -0},
    {0, 1 / std::sqrt(2), -0, -1 / std::sqrt(2)},
  };

  return expected == c_lazy->get();
}

bool it_should_compute_identity_lazy_matrix_tensorial_product() {
  // Given
  const auto m = std::make_shared<ComplexMatrix>(ComplexMatrix{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  });

  // When
  const auto c_lazy = new LazyMatrix(m, m, LazyMatrixOperation::TENSORIAL_PRODUCT);

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
  return equals(expected, c_lazy->get());
}

bool it_should_compute_lazy_matrix_from_lazy_matrices() {
  // Given
  const auto a = std::make_shared<ComplexMatrix>(identity_2x2);
  const auto a_lazy = std::make_shared<LazyMatrix>(a, a, LazyMatrixOperation::TENSORIAL_PRODUCT);
  const auto b_lazy = std::make_shared<LazyMatrix>(a, 1);

  // When
  const auto c_lazy = new LazyMatrix(a_lazy, b_lazy, LazyMatrixOperation::TENSORIAL_PRODUCT);

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
  return equals(expected, c_lazy->get());
}

bool it_should_compute_lazy_matrix_scalar_product() {
  // Given
  const auto m = std::make_shared<ComplexMatrix>(identity_2x2);
  constexpr Complex k = {1, 4};

  // When
  const auto c_lazy = new LazyMatrix(m, k);

  // Then
  const ComplexMatrix expected = {
    {k, 0},
    {0, k}
  };
  return equals(expected, c_lazy->get());
}

int main() {
  int failed = 0;
  if (!it_should_compute_lazy_matrix_tensorial_product()) {
    failed++;
  }
  if (!it_should_compute_identity_lazy_matrix_tensorial_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_matrix_scalar_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_matrix_from_lazy_matrices()) {
    failed++;
  }
  return failed == 0 ? 0 : 1;
}