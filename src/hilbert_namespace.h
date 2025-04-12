#ifndef HILBERT_NAMESPACE_H
#define HILBERT_NAMESPACE_H

#include <vector>
#include <complex>
#include <optional>

using Complex = std::complex<double>;

using ComplexVector = std::vector<Complex>;

using ComplexMatrix = std::vector<ComplexVector>;

using ComplexOptionalMatrix = std::vector<std::vector<std::optional<std::complex<double>>>>;

const ComplexMatrix identity_2x2 = {
  {1, 0},
  {0, 1}
};

const ComplexMatrix hadamard_2x2 = {
  {1 / std::sqrt(2), 1 / std::sqrt(2)},
  {1 / std::sqrt(2), -1 / std::sqrt(2)},
};

#endif // !HILBERT_NAMESPACE_H
