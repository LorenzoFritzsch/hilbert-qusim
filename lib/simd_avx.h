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

#ifndef SIMD_AVX_H
#define SIMD_AVX_H

#include "hilbert_namespace.h"
#include <cstddef>
#include <immintrin.h>

/*
 * This class exposes AVX SIMD instructions, and it's binded only for supported
 * systems.
 * TODO: Implement AVX512 support.
 */
class simd_avx {
public:
  simd_avx() = delete;

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

    vmul(left_real, right_real, ac_vect.data(), ac_vect.size());
    vmul(left_imag, right_imag, bd_vect.data(), bd_vect.size());
    vmul(left_real, right_imag, ad_vect.data(), ad_vect.size());
    vmul(left_imag, right_real, bc_vect.data(), bc_vect.size());

    vsub(ac_vect.data(), bd_vect.data(), result_real, length);
    vadd(ad_vect.data(), bc_vect.data(), result_imag, length);
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

    vsmul(vect_real, k_real, vect_ac.data(), vect_ac.size());
    vsmul(vect_imag, k_imag, vect_bd.data(), vect_bd.size());
    vsmul(vect_real, k_imag, vect_ad.data(), vect_ad.size());
    vsmul(vect_imag, k_real, vect_bc.data(), vect_bc.size());

    vsub(vect_ac.data(), vect_bd.data(), result_real, length);
    vadd(vect_ad.data(), vect_bc.data(), result_imag, length);
  }

  /*
   * Element-wise vector multiplication.
   */
  static void vmul(const __complex_precision *left,
                   const __complex_precision *right,
                   __complex_precision *result, size_t length) {
    size_t i = 0;
    size_t element_size = sizeof(__complex_precision) * 8;
    size_t elements_per_block = 256 / element_size;
    for (; i + elements_per_block <= length; i += elements_per_block) {
      __m256 vec_left = _mm256_loadu_ps(left + i);
      __m256 vec_right = _mm256_loadu_ps(right + i);
      __m256 vec_res = _mm256_mul_ps(vec_left, vec_right);
      _mm256_storeu_ps(result + i, vec_res);
    }
    for (; i < length; i++) {
      result[i] = left[i] * right[i];
    }
  }

  /*
   * Scalar-vector multiplication.
   */
  static void vsmul(const __complex_precision *vect,
                    const __complex_precision &scalar,
                    __complex_precision *result, size_t length) {
    __m256 scalar_vec = _mm256_set1_ps(scalar);

    size_t i = 0;
    size_t element_size = sizeof(__complex_precision) * 8;
    size_t elements_per_block = 256 / element_size;
    for (; i + elements_per_block <= length; i += elements_per_block) {
      __m256 vec = _mm256_loadu_ps(vect + i);
      __m256 res = _mm256_mul_ps(vec, scalar_vec);
      _mm256_storeu_ps(result + i, res);
    }
    for (; i < length; i++) {
      result[i] = vect[i] * scalar;
    }
  }

  /*
   * Element-wise subtraction, performed left - right.
   */
  static void vsub(const __complex_precision *left,
                   const __complex_precision *right,
                   __complex_precision *result, size_t length) {
    size_t i = 0;
    size_t element_size = sizeof(__complex_precision) * 8;
    size_t elements_per_block = 256 / element_size;
    for (; i + elements_per_block <= length; i += elements_per_block) {
      __m256 vec_left = _mm256_loadu_ps(left + i);
      __m256 vec_right = _mm256_loadu_ps(right + i);
      __m256 vec_res = _mm256_sub_ps(vec_left, vec_right);
      _mm256_storeu_ps(result + i, vec_res);
    }
    for (; i < length; i++) {
      result[i] = left[i] - right[i];
    }
  }

  /*
   * Element-wise vector addition.
   */
  static void vadd(const __complex_precision *left,
                   const __complex_precision *right,
                   __complex_precision *result, size_t length) {
    size_t i = 0;
    size_t element_size = sizeof(__complex_precision) * 8;
    size_t elements_per_block = 256 / element_size;
    for (; i + elements_per_block <= length; i += elements_per_block) {
      __m256 vec_left = _mm256_loadu_ps(left + i);
      __m256 vec_right = _mm256_loadu_ps(right + i);
      __m256 vec_res = _mm256_add_ps(vec_left, vec_right);
      _mm256_storeu_ps(result + i, vec_res);
    }
    for (; i < length; i++) {
      result[i] = left[i] + right[i];
    }
  }

  /*
   * Vector horizontal sum.
   */
  static void hsum(const __complex_precision *vect, const size_t length,
                   __complex_precision *result) {
    size_t i = 0;
    size_t element_size = sizeof(__complex_precision) * 8;
    size_t elements_per_block = 256 / element_size;
    __m256 vsum = _mm256_setzero_ps();

    for (; i + elements_per_block <= length; i += elements_per_block) {
      __m256 v = _mm256_loadu_ps(vect + i);
      vsum = _mm256_add_ps(vsum, v);
    }

    __m128 vlow = _mm256_castps256_ps128(vsum);
    __m128 vhigh = _mm256_extractf128_ps(vsum, 1);
    __m128 vsum128 = _mm_add_ps(vlow, vhigh);

    vsum128 = _mm_hadd_ps(vsum128, vsum128);
    vsum128 = _mm_hadd_ps(vsum128, vsum128);

    __complex_precision sum = _mm_cvtss_f32(vsum128);

    for (; i < length; ++i) {
      sum += vect[i];
    }

    result = &sum;
  }
};

#endif // !SIMD_AVX_H
