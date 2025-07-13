#ifndef QUBIT_H
#define QUBIT_H
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include <memory>

class Qubit final {
public:
  Qubit(const Complex alpha, const Complex beta) {
    if (alpha * std::conj(alpha) + beta * std::conj(beta) != Complex(1)) {
      throw std::invalid_argument("Qubit must be normalised");
    }
    this->alpha = alpha;
    this->beta = beta;
  }

  [[nodiscard]] std::unique_ptr<ComplexVectMatrix> to_vector() const {
    return std::make_unique<ComplexVectMatrix>(
        ComplexVector(std::initializer_list{alpha, beta}));
  }

private:
  Complex alpha;
  Complex beta;
};

#endif // !QUBIT_H
