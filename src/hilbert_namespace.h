// Copyright 2025 Lorenzo Fritzsch
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef HILBERT_NAMESPACE_H
#define HILBERT_NAMESPACE_H

#include <complex>
#include <vector>

using __complex_precision = float;

using Complex = std::complex<__complex_precision>;

using ComplexVector = std::vector<Complex>;

using ComplexMatrix = std::vector<ComplexVector>;

// for float: 1e-10
inline bool approx_equal(const Complex &a, const Complex &b,
                         const float tol = 1e-5f) {
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
