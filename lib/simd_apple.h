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

#ifndef SIMD_APPLE_H
#define SIMD_APPLE_H

#include "hilbert_namespace.h"
#include <Accelerate/Accelerate.h>
#include <cstddef>

/*
 * This class exposes Apple Accelerate SIMD instructions, and it's binded only
 * for supported systems.
 */
class simd_apple {
public:
  simd_apple() = delete;

  /*
   * Performs an element-wise complex multiplication given left and right
   * already split in real and imag vectors.
   */
  static void cvmul(const __complex_precision *left_real,
                    const __complex_precision *left_imag,
                    const __complex_precision *right_real,
                    const __complex_precision *right_imag,
                    __complex_precision *result_real,
                    __complex_precision *result_imag, size_t length) {
    // Considering the formula (a + bi)(c + di) = (ac - bd) + i(ad + bc):
    std::vector<__complex_precision> ac_vect(length), bd_vect(length),
        ad_vect(length), bc_vect(length);

    vDSP_vmul(left_real, 1, right_real, 1, ac_vect.data(), 1, ac_vect.size());
    vDSP_vmul(left_imag, 1, right_imag, 1, bd_vect.data(), 1, bd_vect.size());
    vDSP_vmul(left_real, 1, right_imag, 1, ad_vect.data(), 1, ad_vect.size());
    vDSP_vmul(left_imag, 1, right_real, 1, bc_vect.data(), 1, bc_vect.size());

    vDSP_vsub(bd_vect.data(), 1, ac_vect.data(), 1, result_real, 1, length);
    vDSP_vadd(ad_vect.data(), 1, bc_vect.data(), 1, result_imag, 1, length);
  }

  /*
   * Performs the scalar product between a complex number and a complex vector.
   * Both the complex number and the complex vector are passed to this method
   * already split.
   */
  static void cvsmul(const __complex_precision &k_real,
                     const __complex_precision &k_imag,
                     const __complex_precision *vect_real,
                     const __complex_precision *vect_imag,
                     __complex_precision *result_real,
                     __complex_precision *result_imag, const size_t length) {
    // Considering the formula (a + bi)(c + di) = (ac - bd) + i(ad + bc):
    std::vector<__complex_precision> vect_ac(length), vect_bd(length),
        vect_ad(length), vect_bc(length);

    vDSP_vsmul(vect_real, 1, &k_real, vect_ac.data(), 1, vect_ac.size());
    vDSP_vsmul(vect_imag, 1, &k_imag, vect_bd.data(), 1, vect_bd.size());
    vDSP_vsmul(vect_real, 1, &k_imag, vect_ad.data(), 1, vect_ad.size());
    vDSP_vsmul(vect_imag, 1, &k_real, vect_bc.data(), 1, vect_bc.size());

    vDSP_vsub(vect_bd.data(), 1, vect_ac.data(), 1, result_real, 1, length);
    vDSP_vadd(vect_ad.data(), 1, vect_bc.data(), 1, result_imag, 1, length);
  }

  /*
   * Element-wise vector multiplication.
   */
  static void vmul(const __complex_precision *left,
                   const __complex_precision *right,
                   __complex_precision *result, size_t length) {
    vDSP_vmul(left, 1, right, 1, result, 1, length);
  }

  /*
   * Scalar-vector multiplication.
   */
  static void vsmul(const __complex_precision *vect,
                    const __complex_precision *scalar,
                    __complex_precision *result, size_t length) {
    vDSP_vsmul(vect, 1, scalar, result, 1, length);
  }

  /*
   * Element-wise subtraction, performed left - right.
   */
  static void vsub(const __complex_precision *left,
                   const __complex_precision *right,
                   __complex_precision *result, size_t length) {
    vDSP_vsub(right, 1, left, 1, result, 1, length);
  }

  /*
   * Element-wise vector addition.
   */
  static void vadd(const __complex_precision *left,
                   const __complex_precision *right,
                   __complex_precision *result, size_t length) {
    vDSP_vadd(left, 1, right, 1, result, 1, length);
  }

  /*
   * Vector horizontal sum.
   */
  static void hsum(const __complex_precision *vect, const size_t length,
                   __complex_precision *result) {
    vDSP_sve(vect, 1, result, length);
  }
};

#endif // !SIMD_APPLE_H
