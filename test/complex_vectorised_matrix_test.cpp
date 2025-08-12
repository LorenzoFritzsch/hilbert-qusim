#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "hilbert_namespace_test.h"
#include <memory>

bool it_should_create_vectorised_matrix() {
  // When
  const auto m_vectorised = std::make_unique<ComplexVectMatrix>(identity_2x2);

  // Then
  return verify_identity_matrix(*m_vectorised, 2);
}

int main() {
  int total = 0;
  int failed = 0;

  run_test("it_should_create_vectorised_matrix",
           it_should_create_vectorised_matrix, failed, total);

  test_resumen(failed, total);
  return failed == 0 ? 0 : 1;
}
