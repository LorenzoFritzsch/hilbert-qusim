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
#include <Accelerate/Accelerate.h>
#else
#include <cstddef>
#include <immintrin.h>
#endif

class simd {
public:
  /*
   * SIMD element-wise complex vector multiplication
   */
  static std::unique_ptr<ComplexVectSplit>
  cvmul(const ComplexVectSplit &left, const ComplexVectSplit &right) {

    // Considering the formula (a + bi)(c + di) = (ac - bd) + i(ad + bc):
    std::vector<__complex_precision> ac_vect(left.size()), bd_vect(left.size()),
        ad_vect(left.size()), bc_vect(left.size());

    std::vector<__complex_precision> result_real(left.size());
    std::vector<__complex_precision> result_imag(left.size());

    const auto left_real = left.real();
    const auto left_imag = left.imag();
    const auto right_real = right.real();
    const auto right_imag = right.imag();

#ifdef __APPLE__
    vDSP_vmul(left_real.data(), 1, right_real.data(), 1, ac_vect.data(), 1,
              ac_vect.size());
    vDSP_vmul(left_imag.data(), 1, right_imag.data(), 1, bd_vect.data(), 1,
              bd_vect.size());
    vDSP_vmul(left_real.data(), 1, right_imag.data(), 1, ad_vect.data(), 1,
              ad_vect.size());
    vDSP_vmul(left_imag.data(), 1, right_real.data(), 1, bc_vect.data(), 1,
              bc_vect.size());

    vDSP_vsub(bd_vect.data(), 1, ac_vect.data(), 1, result_real.data(), 1,
              result_real.size());
    vDSP_vadd(ad_vect.data(), 1, bc_vect.data(), 1, result_imag.data(), 1,
              result_imag.size());
#else
    vmul_avx(left_real.data(), right_real.data(), ac_vect.data(),
             ac_vect.size());
    vmul_avx(left_imag.data(), right_imag.data(), bd_vect.data(),
             bd_vect.size());
    vmul_avx(left_real.data(), right_imag.data(), ad_vect.data(),
             ad_vect.size());
    vmul_avx(left_imag.data(), right_real.data(), bc_vect.data(),
             bc_vect.size());

    vsub_avx(ac_vect.data(), bd_vect.data(), result_real.data(),
             result_real.size());
    vadd_avx(ad_vect.data(), bc_vect.data(), result_imag.data(),
             result_imag.size());
#endif

    return std::make_unique<ComplexVectSplit>(result_real, result_imag);
  }

  /*
   * SIMD sum over each element of the vector.
   */
  static Complex cvsve(const ComplexVectSplit &vect) {
    __complex_precision result_real = 0;
    __complex_precision result_imag = 0;
    const auto vect_size = vect.size();

#ifdef __APPLE__
    vDSP_sve(vect.real().data(), 1, &result_real, vect_size);
    vDSP_sve(vect.imag().data(), 1, &result_imag, vect_size);
#else
    result_real = hsum_avx(vect.real().data(), vect_size);
    result_imag = hsum_avx(vect.imag().data(), vect_size);
#endif

    return Complex(result_real, result_imag);
  }

  /*
   * SIMD element-wise sum of two vectors.
   */
  static std::unique_ptr<ComplexVectSplit>
  cvadd(const ComplexVectSplit &left, const ComplexVectSplit &right) {
    std::vector<__complex_precision> result_real(left.size()),
        result_imag(left.size());

#ifdef __APPLE__
    vDSP_vadd(left.real().data(), 1, right.real().data(), 1, result_real.data(),
              1, result_real.size());
    vDSP_vadd(left.imag().data(), 1, right.imag().data(), 1, result_imag.data(),
              1, result_imag.size());
#else
    vadd_avx(left.real().data(), right.real().data(), result_real.data(),
             result_real.size());
    vadd_avx(left.imag().data(), right.imag().data(), result_imag.data(),
             result_imag.size());
#endif

    return std::make_unique<ComplexVectSplit>(result_real, result_imag);
  }

  /*
   * SIMD element-wise subtraction of two vectors.
   */
  static std::unique_ptr<ComplexVectSplit>
  cvsub(const ComplexVectSplit &left, const ComplexVectSplit &right) {
    std::vector<__complex_precision> result_real(left.size()),
        result_imag(right.size());

#ifdef __APPLE__
    vDSP_vsub(right.real().data(), 1, left.real().data(), 1, result_real.data(),
              1, result_real.size());
    vDSP_vsub(right.imag().data(), 1, left.imag().data(), 1, result_imag.data(),
              1, result_imag.size());
#else
    vsub_avx(left.real().data(), right.real().data(), result_real.data(),
             result_real.size());
    vsub_avx(left.imag().data(), right.imag().data(), result_imag.data(),
             result_imag.size());
#endif

    return std::make_unique<ComplexVectSplit>(result_real, result_imag);
  }

  /*
   * SIMD scalar product.
   */
  static std::unique_ptr<ComplexVectSplit> cvsmul(const ComplexVectSplit &vect,
                                                  const Complex &k) {
    const auto vect_real = vect.real();
    const auto vect_imag = vect.imag();
    const auto k_real = static_cast<__complex_precision>(k.real());
    const auto k_imag = static_cast<__complex_precision>(k.imag());

    // Considering the formula (a + bi)(c + di) = (ac - bd) + i(ad + bc):
    std::vector<__complex_precision> vect_ac(vect.size()), vect_bd(vect.size()),
        vect_ad(vect.size()), vect_bc(vect.size());

    std::vector<__complex_precision> result_real(vect.size()),
        result_imag(vect.size());

#ifdef __APPLE__
    vDSP_vsmul(vect_real.data(), 1, &k_real, vect_ac.data(), 1, vect_ac.size());
    vDSP_vsmul(vect_imag.data(), 1, &k_imag, vect_bd.data(), 1, vect_bd.size());
    vDSP_vsmul(vect_real.data(), 1, &k_imag, vect_ad.data(), 1, vect_ad.size());
    vDSP_vsmul(vect_imag.data(), 1, &k_real, vect_bc.data(), 1, vect_bc.size());

    vDSP_vsub(vect_bd.data(), 1, vect_ac.data(), 1, result_real.data(), 1,
              result_real.size());
    vDSP_vadd(vect_ad.data(), 1, vect_bc.data(), 1, result_imag.data(), 1,
              result_imag.size());
#else
    vsmul_avx(vect_real.data(), k_real, vect_ac.data(), vect_ac.size());
    vsmul_avx(vect_imag.data(), k_imag, vect_bd.data(), vect_bd.size());
    vsmul_avx(vect_real.data(), k_imag, vect_ad.data(), vect_ad.size());
    vsmul_avx(vect_imag.data(), k_real, vect_bc.data(), vect_bc.size());

    vsub_avx(vect_ac.data(), vect_bd.data(), result_real.data(),
             result_real.size());
    vadd_avx(vect_ad.data(), vect_bc.data(), result_imag.data(),
             result_imag.size());
#endif

    return std::make_unique<ComplexVectSplit>(result_real, result_imag);
  }

#ifndef __APPLE__
private:
  /*
   * SIMD AVX vector multiplication.
   */
  static void vmul_avx(const __complex_precision *left,
                       const __complex_precision *right,
                       __complex_precision *result, size_t length) {
    size_t i = 0;
    const size_t element_size = sizeof(__complex_precision) * 8;
    const size_t elements_per_block = 256 / element_size;
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
   * SIMD AVX scalar-vector multiplication.
   */
  static void vsmul_avx(const __complex_precision *vect,
                        const __complex_precision &scalar,
                        __complex_precision *result, size_t length) {
    __m256 scalar_vec = _mm256_set1_ps(scalar);

    size_t i = 0;
    const size_t element_size = sizeof(__complex_precision) * 8;
    const size_t elements_per_block = 256 / element_size;
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
   * SIMD AVX vector subtraction, performed left - right.
   */
  static void vsub_avx(const __complex_precision *left,
                       const __complex_precision *right,
                       __complex_precision *result, size_t length) {
    size_t i = 0;
    const size_t element_size = sizeof(__complex_precision) * 8;
    const size_t elements_per_block = 256 / element_size;
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
   * SIMD AVX vector addition.
   */
  static void vadd_avx(const __complex_precision *left,
                       const __complex_precision *right,
                       __complex_precision *result, size_t length) {
    size_t i = 0;
    const size_t element_size = sizeof(__complex_precision) * 8;
    const size_t elements_per_block = 256 / element_size;
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
   * SIMD AVX horizontal sum.
   */
  static __complex_precision hsum_avx(const __complex_precision *vect,
                                      size_t length) {
    size_t i = 0;
    const size_t element_size = sizeof(__complex_precision) * 8;
    const size_t elements_per_block = 256 / element_size;
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

    float sum = _mm_cvtss_f32(vsum128);

    for (; i < length; ++i) {
      sum += vect[i];
    }

    return sum;
  }
#endif
};

#endif // !SIMD_H
