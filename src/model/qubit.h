#ifndef QUBIT_QUBIT_H
#define QUBIT_QUBIT_H

#include "hilbert_namespace.h"

class Qubit {
public:
  Complex alpha;
  Complex beta;

  Qubit();
  Qubit(Complex alpha, Complex beta);

  ComplexVector toVector();
};

#endif // !QUBIT_QUBIT_H
