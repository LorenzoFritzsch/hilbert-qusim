#include "algebra_engine.h"

#include "lazy_matrix_operation_identity.h"
#include "lazy_matrix_operation_lazy_tensor_product.h"

std::unique_ptr<LazyMatrix> AlgebraEngine::to_lazy(std::unique_ptr<ComplexMatrix> a) {
  return std::make_unique<LazyMatrix>(std::make_unique<LazyMatrixOperationIdentity>(std::move(a)));
}

ComplexMatrix AlgebraEngine::conjugate_transpose(const ComplexMatrix &mat) {
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

Complex AlgebraEngine::inner_product(const ComplexVector &a, const ComplexVector &b) {
  const auto aMatrix = ComplexMatrix(1, a);
  const auto bMatrix = ComplexMatrix(1, b);
  return inner_product(aMatrix, bMatrix);
}

Complex AlgebraEngine::inner_product(const ComplexMatrix &a, const ComplexMatrix &b) {
  Complex result = 0;
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++) {
      result += std::conj(a[i][j]) * b[i][j];
    }
  }
  return result;
}

std::unique_ptr<LazyMatrix> AlgebraEngine::tensorial_product(std::unique_ptr<ComplexMatrix> a, const int times) {
  auto lazy_a = to_lazy(std::move(a));
  if (times == 1) {
    return lazy_a;
  }
  auto operation = std::make_unique<LazyMatrixOperationLazyTensorProduct>(
    std::make_unique<LazyMatrix>(*lazy_a), std::make_unique<LazyMatrix>(*lazy_a));
  for (int _ = 2; _ < times; _++) {
    operation = std::make_unique<LazyMatrixOperationLazyTensorProduct>(
      std::make_unique<LazyMatrix>(std::move(operation)), std::make_unique<LazyMatrix>(*lazy_a));
  }
  return std::make_unique<LazyMatrix>(std::move(operation));
}