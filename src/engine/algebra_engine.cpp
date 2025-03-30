#include "algebra_engine.h"

Complex AlgebraEngine::conjugate(Complex *c) { return (c->real(), -c->imag()); }

ComplexMatrix AlgebraEngine::conjugateTranspose(ComplexMatrix *mat) {
  int initial_n = mat->size();
  int initial_m = mat->begin()->size();

  ComplexMatrix result(initial_m, ComplexVector(initial_n));

  ComplexMatrix mat_deref = *mat;
  for (int n = 0; n < initial_n; n++) {
    for (int m = 0; m < initial_m; m++) {
      result[m][n] = conjugate(&mat_deref[n][m]);
    }
  }

  return result;
}

Complex AlgebraEngine::multiply(Complex *a, Complex *b) {
  // TODO: Review warning
  return (a->real() * b->real() - a->imag() * b->imag(),
          a->real() * b->imag() + a->imag() * b->real());
}

ComplexVector AlgebraEngine::multiply(ComplexVector *v, Complex *k) {
  ComplexVector result = ComplexVector(v->size());
  ComplexVector v_deref = *v;
  for (int i = 0; i < v->size(); i++) {
    result[i] = multiply(&v_deref[i], k);
  }
  return result;
}
