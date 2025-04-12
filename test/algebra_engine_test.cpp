// TODO: Fix CMake
#include "../src/engine/algebra_engine.h"

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

  return failed == 0 ? 0 : 1;
}
