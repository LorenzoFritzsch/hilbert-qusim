#ifndef HILBERT_NAMESPACE_H
#define HILBERT_NAMESPACE_H

#include <complex>
#include <vector>

using Complex = std::complex<double>;

using ComplexVector = std::vector<Complex>;

using ComplexMatrix = std::vector<ComplexVector>;

inline bool approx_equal(const Complex &a, const Complex &b,
                         const double tol = 1e-10) {
  return std::abs(a - b) < tol;
}

const ComplexVector ket_0 = {1, 0};
const ComplexVector ket_1 = {0, 1};

const ComplexMatrix identity_2x2 = {{1, 0}, {0, 1}};

const ComplexMatrix hadamard_2x2 = {
    {1 / std::sqrt(2), 1 / std::sqrt(2)},
    {1 / std::sqrt(2), -1 / std::sqrt(2)},
};

const ComplexMatrix pauli_x = {{0, 1}, {1, 0}};

const ComplexMatrix pauli_y = {{0, Complex(0, -1)}, {Complex(0, 1), 0}};

const ComplexMatrix pauli_z = {{1, 0}, {0, -1}};

#endif // !HILBERT_NAMESPACE_H
