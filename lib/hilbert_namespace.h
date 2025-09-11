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

inline bool approx_equal(const Complex &a, const Complex &b) {
  __complex_precision tol = 1e-5;
  return std::abs(a - b) < tol;
}

/*
 * Calculates the squared norm.
 */
inline Complex norm(const Complex &a, const Complex &b) {
  return static_cast<Complex>(std::norm(a) + std::norm(b));
}

#endif // !HILBERT_NAMESPACE_H
