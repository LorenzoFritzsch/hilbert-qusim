#include "algebra_engine.h"

ComplexMatrix AlgebraEngine::conjugateTranspose(const ComplexMatrix &mat) {
  const auto initial_n = mat.size();
  const auto initial_m = mat.begin()->size();

  ComplexMatrix result(initial_m, ComplexVector(initial_n));

  for (int n = 0; n < initial_n; n++) {
    for (int m = 0; m < initial_m; m++) {
      result[m][n] = std::conj(mat[n][m]);
    }
  }

  return result;
}

ComplexVector AlgebraEngine::multiply(const ComplexVector &v, const Complex &k) {
  auto result = ComplexVector(v.size());
  for (int i = 0; i < v.size(); i++) {
    result[i] = v[i] * k;
  }
  return result;
}

Complex AlgebraEngine::innerProduct(const ComplexVector &a, const ComplexVector &b) {
  const auto aMatrix = ComplexMatrix(1, a);
  const auto bMatrix = ComplexMatrix(1, b);
  return innerProduct(aMatrix, bMatrix);
}

Complex AlgebraEngine::innerProduct(const ComplexMatrix &a, const ComplexMatrix &b) {
  Complex result = 0;
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++) {
      result += std::conj(a[i][j]) * b[i][j];
    }
  }
  return result;
}

std::shared_ptr<LazyMatrix> AlgebraEngine::tensorialProduct(const std::shared_ptr<ComplexMatrix>& a, const int times) {
  auto result = std::make_shared<LazyMatrix>(a, a, LazyMatrixOperation::TENSORIAL_PRODUCT);
  auto lazyA = std::make_shared<LazyMatrix>(a, 1);
  // Starts at 2, because `a` was already multiplied with itself once, missing products are then: times - 2.
  for (int _ = 2; _ < times; _++) {
    result = std::make_shared<LazyMatrix>(result, lazyA, LazyMatrixOperation::TENSORIAL_PRODUCT);
  }
  return result;
}
