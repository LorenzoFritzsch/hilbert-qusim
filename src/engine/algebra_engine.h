#ifndef ALGEBRA_ENGINE_H
#define ALGEBRA_ENGINE_H

#include "hilbert_namespace.h"

class AlgebraEngine {
public:
  AlgebraEngine() {}

  Complex conjugate(Complex *c);

  ComplexMatrix conjugateTranspose(ComplexMatrix *mat);

  Complex multiply(Complex *a, Complex *b);

  ComplexVector multiply(ComplexVector *v, Complex *k);

  ComplexVector multiply(ComplexVector *a, ComplexVector *b);

  ComplexMatrix tensorialProduct(ComplexMatrix *a, ComplexMatrix *b);
};

#endif // !ALGEBRA_ENGINE_H
