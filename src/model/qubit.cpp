#include "qubit.h"

Qubit::Qubit(std::complex<double> alpha, std::complex<double> beta) {
  this->alpha = alpha;
  this->beta = beta;
}

std::vector<std::complex<double>> Qubit::toVector() { return {alpha, beta}; }
