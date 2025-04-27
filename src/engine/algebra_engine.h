#ifndef ALGEBRA_ENGINE_H
#define ALGEBRA_ENGINE_H

#include <memory>
#include "hilbert_namespace.h"
#include "lazy_matrix.h"

class AlgebraEngine {
public:
  AlgebraEngine() = default;

  static ComplexMatrix conjugate_transpose(const ComplexMatrix &mat);

  static ComplexVector multiply(const ComplexVector &v, const Complex &k);

  static Complex inner_product(const ComplexVector &a, const ComplexVector &b);

  static Complex inner_product(const ComplexMatrix &a, const ComplexMatrix &b);

  static std::unique_ptr<LazyMatrix> tensorial_product(std::unique_ptr<ComplexMatrix> a, int times);
};

#endif // !ALGEBRA_ENGINE_H
