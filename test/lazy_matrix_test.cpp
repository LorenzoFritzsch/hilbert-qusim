#include "lazy_matrix.h"
#include "lazy_matrix_operation_tensor_product.h"
#include "lazy_matrix_operation_cast.h"
#include "lazy_matrix_operation_lazy_outer_product.h"
#include "lazy_matrix_operation_lazy_scalar_product.h"
#include "lazy_matrix_operation_lazy_tensor_product.h"
#include "lazy_matrix_operation_scalar_product.h"
#include "lazy_vector.h"
#include "lazy_vector_operation_cast.h"

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

bool it_should_compute_tensor_product() {
  // Given
  auto a = std::make_unique<ComplexMatrix>(hadamard_2x2);
  auto b = std::make_unique<ComplexMatrix>(identity_2x2);
  auto operation = std::make_unique<LazyMatrixOperationTensorProduct>(std::move(a), std::move(b));

  // When
  const auto c_lazy = std::make_unique<LazyMatrix>(std::move(operation));

  // Then
  const ComplexMatrix expected = {
    {1 / std::sqrt(2), 0, 1 / std::sqrt(2), 0},
    {0, 1 / std::sqrt(2), 0, 1 / std::sqrt(2)},
    {1 / std::sqrt(2), 0, -1 / std::sqrt(2), -0},
    {0, 1 / std::sqrt(2), -0, -1 / std::sqrt(2)},
  };

  return expected == c_lazy->get();
}

bool it_should_compute_identity_tensor_product() {
  // Given
  auto m_1 = std::make_unique<ComplexMatrix>(ComplexMatrix{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  });
  auto m_2 = std::make_unique<ComplexMatrix>(ComplexMatrix{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  });
  auto operation = std::make_unique<LazyMatrixOperationTensorProduct>(std::move(m_1), std::move(m_2));

  // When
  const auto c_lazy = std::make_unique<LazyMatrix>(std::move(operation));

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

bool it_should_compute_lazy_matrix_from_matrix() {
  // Given
  auto m = std::make_unique<ComplexMatrix>(identity_2x2);
  auto operation = std::make_unique<LazyMatrixOperationCast>(std::move(m));

  // When
  const auto c_lazy = std::make_unique<LazyMatrix>(std::move(operation));

  // Then
  const auto expected = identity_2x2;
  return equals(expected, c_lazy->get());
}

bool it_should_compute_lazy_tensor_product() {
  // Given
  auto a = std::make_unique<ComplexMatrix>(ComplexMatrix({
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }));
  auto operation_a = std::make_unique<LazyMatrixOperationCast>(std::move(a));
  auto a_lazy = std::make_unique<LazyMatrix>(std::move(operation_a));
  auto b = std::make_unique<ComplexMatrix>(identity_2x2);
  auto operation_b = std::make_unique<LazyMatrixOperationCast>(std::move(b));
  auto b_lazy = std::make_unique<LazyMatrix>(std::move(operation_b));
  auto operation = std::make_unique<LazyMatrixOperationLazyTensorProduct>(std::move(a_lazy), std::move(b_lazy));

  // When
  const auto c_lazy = std::make_unique<LazyMatrix>(std::move(operation));

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

bool it_should_compute_scalar_product() {
  // Given
  auto m = std::make_unique<ComplexMatrix>(identity_2x2);
  constexpr Complex k = {1, 4};
  auto operation = std::make_unique<LazyMatrixOperationScalarProduct>(std::move(m), std::make_unique<Complex>(k));

  // When
  const auto c_lazy = std::make_unique<LazyMatrix>(std::move(operation));

  // Then
  const ComplexMatrix expected = {
    {k, 0},
    {0, k}
  };
  return equals(expected, c_lazy->get());
}

bool it_should_compute_lazy_scalar_product() {
  // Given
  auto a = std::make_unique<ComplexMatrix>(identity_2x2);
  auto operation_a = std::make_unique<LazyMatrixOperationCast>(std::move(a));
  auto a_lazy = std::make_unique<LazyMatrix>(std::move(operation_a));
  constexpr Complex k = {1, 4};
  auto operation = std::make_unique<LazyMatrixOperationLazyScalarProduct>(std::move(a_lazy), std::make_unique<Complex>(k));

  // When
  const auto c_lazy = std::make_unique<LazyMatrix>(std::move(operation));

  //
  const ComplexMatrix expected = {
    {k, 0},
    {0, k}
  };
  return equals(expected, c_lazy->get());
}

bool it_should_compute_lazy_outer_product() {
  // Given
  auto v1 = std::make_unique<ComplexVector>(ket_0);
  auto v1_lazy = std::make_unique<LazyVector>(std::make_unique<LazyVectorOperationCast>(std::move(v1)));
  auto v2 = std::make_unique<ComplexVector>(ket_1);
  auto v2_lazy = std::make_unique<LazyVector>(std::make_unique<LazyVectorOperationCast>(std::move(v2)));
  auto operation = std::make_unique<LazyMatrixOperationLazyOuterProduct>(std::move(v1_lazy), std::move(v2_lazy));

  // When
  const auto c_lazy = std::make_unique<LazyMatrix>(std::move(operation));

  // Then
  const ComplexMatrix expected = {
    {0, 1},
    {0, 0}
  };
  return equals(expected, c_lazy->get());
}

int main() {
  int failed = 0;
  if (!it_should_compute_tensor_product()) {
    failed++;
  }
  if (!it_should_compute_identity_tensor_product()) {
    failed++;
  }
  if (!it_should_compute_scalar_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_matrix_from_matrix()) {
    failed++;
  }
  if (!it_should_compute_lazy_tensor_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_scalar_product()) {
    failed++;
  }
  if (!it_should_compute_lazy_outer_product()) {
    failed++;
  }
  return failed == 0 ? 0 : 1;
}