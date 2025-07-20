#ifndef QUBIT_H
#define QUBIT_H
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include <memory>
#include <stdexcept>

class Qubit final {
public:
  Qubit(const Complex alpha, const Complex beta) {
    auto norm = static_cast<Complex>(std::norm(alpha) + std::norm(beta));
    if (!approx_equal(norm, Complex(1))) {
      throw std::invalid_argument("Qubit must be normalised (norm = " +
                                  std::to_string(norm.real()) + ")");
    }
    this->alpha = alpha;
    this->beta = beta;
  }

  explicit Qubit(std::unique_ptr<OpMember> op_member) {
    if (op_member->row_size() != 1 || op_member->column_size() != 2) {
      throw std::invalid_argument(
          "OpMember must represent a 2 elements vector");
    }
    auto alpha = op_member->get(0, 0);
    auto beta = op_member->get(0, 1);
    auto norm = alpha * std::conj(alpha) + beta * std::conj(beta);
    if (!approx_equal(norm, Complex(1))) {
      throw std::invalid_argument("Qubit must be normalised (norm = " +
                                  std::to_string(norm.real()) + ")");
    }
    this->alpha = alpha;
    this->beta = beta;
  }

  Qubit() {
    this->alpha = 1;
    this->beta = 0;
  }

  bool operator==(const Qubit &other) const {
    return approx_equal(alpha, other.alpha) && approx_equal(beta, other.beta);
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
