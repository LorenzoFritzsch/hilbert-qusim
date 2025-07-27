
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "hilbert_namespace_test.h"
#include <iostream>
#include <memory>

bool verify_identity_matrix(const ComplexVectMatrix &matrix) {
  if (matrix.row_size() != matrix.column_size()) {
    return false;
  }
  for (auto m = 0; m < matrix.row_size(); m++) {
    for (auto n = 0; n < matrix.column_size(); n++) {
      if (m == n) {
        if (matrix.get(m, n) != Complex(1, 0)) {
          return false;
        }
      } else {
        if (matrix.get(m, n) != Complex(0, 0)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool it_should_create_vectorised_matrix() {
  // When
  const auto m_vectorised = std::make_unique<ComplexVectMatrix>(identity_2x2);

  // Then
  return verify_identity_matrix(*m_vectorised);
}

int main() {
  int total = 0;
  int failed = 0;

  if (!run_test("it_should_create_vectorised_matrix",
                it_should_create_vectorised_matrix)) {
    failed++;
  }
  total++;

  std::cout << "\033[1mRun: " << total << ", failed: " << failed << "\033[0m"
            << std::endl;
  return failed == 0 ? 0 : 1;
}
