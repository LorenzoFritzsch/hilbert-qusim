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

#ifndef SIMD_H
#define SIMD_H

#include "complex_vector_split.h"
#include <memory.h>

#ifdef __APPLE__
#include "simd_apple.h"
using __simd__ = simd_apple;
#else
#include "simd_avx.h"
using __simd__ = simd_avx;
#endif

class simd {
public:
  simd() = delete;

  /*
   * SIMD element-wise complex vector multiplication
   */
  static std::unique_ptr<ComplexVectSplit>
  cvmul(const ComplexVectSplit &left, const ComplexVectSplit &right) {

    std::vector<__complex_precision> result_real(left.size());
    std::vector<__complex_precision> result_imag(left.size());

    auto left_real = *left.real();
    auto left_imag = *left.imag();
    auto right_real = *right.real();
    auto right_imag = *right.imag();

    __simd__::cvmul(left_real.data(), left_imag.data(), right_real.data(),
                    right_imag.data(), result_real.data(), result_imag.data(),
                    left.size());

    return std::make_unique<ComplexVectSplit>(result_real, result_imag);
  }

  /*
   * SIMD sum over each element of the vector.
   */
  static Complex cvsve(const ComplexVectSplit &vect) {
    __complex_precision result_real = 0;
    __complex_precision result_imag = 0;
    auto vect_size = vect.size();

    __simd__::hsum(vect.real()->data(), vect_size, &result_real);
    __simd__::hsum(vect.imag()->data(), vect_size, &result_imag);

    return Complex(result_real, result_imag);
  }

  /*
   * SIMD element-wise sum of two vectors.
   */
  static std::unique_ptr<ComplexVectSplit>
  cvadd(const ComplexVectSplit &left, const ComplexVectSplit &right) {
    std::vector<__complex_precision> result_real(left.size()),
        result_imag(left.size());

    __simd__::vadd(left.real()->data(), right.real()->data(),
                   result_real.data(), result_real.size());
    __simd__::vadd(left.imag()->data(), right.imag()->data(),
                   result_imag.data(), result_imag.size());

    return std::make_unique<ComplexVectSplit>(result_real, result_imag);
  }

  /*
   * SIMD element-wise subtraction of two vectors.
   */
  static std::unique_ptr<ComplexVectSplit>
  cvsub(const ComplexVectSplit &left, const ComplexVectSplit &right) {
    std::vector<__complex_precision> result_real(left.size()),
        result_imag(right.size());

    __simd__::vsub(left.real()->data(), right.real()->data(),
                   result_real.data(), result_real.size());
    __simd__::vsub(left.imag()->data(), right.imag()->data(),
                   result_imag.data(), result_imag.size());

    return std::make_unique<ComplexVectSplit>(result_real, result_imag);
  }

  /*
   * SIMD scalar product.
   */
  static std::unique_ptr<ComplexVectSplit> cvsmul(const ComplexVectSplit &vect,
                                                  const Complex &k) {
    auto vect_real = *vect.real();
    auto vect_imag = *vect.imag();
    auto k_real = static_cast<__complex_precision>(k.real());
    auto k_imag = static_cast<__complex_precision>(k.imag());

    std::vector<__complex_precision> result_real(vect.size()),
        result_imag(vect.size());

    __simd__::cvsmul(k_real, k_imag, vect_real.data(), vect_imag.data(),
                     result_real.data(), result_imag.data(),
                     result_real.size());

    return std::make_unique<ComplexVectSplit>(result_real, result_imag);
  }
};

#endif // !SIMD_H
