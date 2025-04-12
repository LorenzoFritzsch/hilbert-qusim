#include "qubit.h"

Qubit::Qubit() {
  this->alpha = 1;
  this->beta = 0;
}

Qubit::Qubit(const std::complex<double> alpha, const std::complex<double> beta) : alpha(alpha), beta(beta) {}

std::vector<std::complex<double>> Qubit::toVector() { return {alpha, beta}; }
