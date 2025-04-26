#ifndef ALGEBRA_ENGINE_H
#define ALGEBRA_ENGINE_H

#include "hilbert_namespace.h"
#include "lazy_matrix.h"

class AlgebraEngine {
public:
  AlgebraEngine() = default;

  static ComplexMatrix conjugateTranspose(const ComplexMatrix &mat);

  static ComplexVector multiply(const ComplexVector &v, const Complex &k);

  static Complex innerProduct(const ComplexVector &a, const ComplexVector &b);

  static Complex innerProduct(const ComplexMatrix &a, const ComplexMatrix &b);

  static std::shared_ptr<LazyMatrix> tensorialProduct(const std::shared_ptr<ComplexMatrix>& a, int times);
};

#endif // !ALGEBRA_ENGINE_H
