// TODO: Fix CMake
#include "../src/engine/algebra_engine.h"
#include <iostream>

bool verifyIdentityMatrix(const ComplexMatrix &matrix) {
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

bool itShouldComputeConjugateTranspose() {
  // Given
  auto complexMatrix = ComplexMatrix(2, ComplexVector(2, 1));
  complexMatrix[0][0] = Complex(1, 1);
  complexMatrix[0][1] = Complex(1, 2);
  complexMatrix[1][0] = Complex(2, 1);
  complexMatrix[1][1] = Complex(2, 2);

  // When
  auto conjugateTranspose = AlgebraEngine::conjugateTranspose(complexMatrix);
  conjugateTranspose = AlgebraEngine::conjugateTranspose(conjugateTranspose);

  // Then - Applying the conjugateTranspose twice should return the original matrix
  return complexMatrix == conjugateTranspose;
}

bool itShouldMultiplyVectorByScalar() {
  // Given
  const auto complexVector = ComplexVector(2, 1);
  constexpr auto scalar = Complex(2, 2);

  // When
  const auto result = AlgebraEngine::multiply(complexVector, scalar);

  // Then
  return result[0] == Complex(2, 2) && result[1] == Complex(2, 2);
}

bool itShouldComputeInnerProduct_betweenTwoVectors() {
  // Given
  const auto complexVectorA = ComplexVector(1, 1);
  const auto complexVectorB = ComplexVector(1, 1);
  const auto complexVectorC = ComplexVector(1, 0);

  // When
  const auto resultAScalarB = AlgebraEngine::innerProduct(complexVectorA, complexVectorB);
  const auto resultAScalarC = AlgebraEngine::innerProduct(complexVectorA, complexVectorC);

  // Then
  const bool isScalarBetweenOrthogonalVectorsZero = resultAScalarC == Complex(0, 0);
  const bool isScalarBetweenEqualVectorsOne = resultAScalarB == Complex(1, 0);
  return isScalarBetweenOrthogonalVectorsZero && isScalarBetweenEqualVectorsOne;
}

bool itShouldComputeMatrixPower() {
  // Given
  const auto a = std::make_shared<ComplexMatrix>(identity_2x2);
  constexpr auto times = 8;

  // When
  const auto result = AlgebraEngine::tensorialProduct(a, times);

  // Then
  const auto actual = result->get();
  return verifyIdentityMatrix(actual)
         && static_cast<int>(actual.size()) == std::pow(2, times);
}

int main() {
  int failed = 0;

  if (!itShouldComputeConjugateTranspose()) {
    failed += 1;
  }
  if (!itShouldMultiplyVectorByScalar()) {
    failed += 1;
  }
  if (!itShouldComputeInnerProduct_betweenTwoVectors()) {
    failed += 1;
  }
  if (!itShouldComputeMatrixPower()) {
    failed += 1;
  }

  return failed == 0 ? 0 : 1;
}
