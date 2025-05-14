#ifndef QUBIT_H
#define QUBIT_H
#include "hilbert_namespace.h"
#include <memory>

class Qubit {
public:
  Qubit(const Complex alpha, const Complex beta) {
    if (alpha * std::conj(alpha) + beta * std::conj(beta) != Complex(1)) {
      throw std::invalid_argument("Qubit must be normalised");
    }
    this->alpha = alpha;
    this->beta = beta;
  }

  [[nodiscard]] std::unique_ptr<ComplexVector> to_vector() const;
private:
  Complex alpha;
  Complex beta;
};

#endif // !QUBIT_H
