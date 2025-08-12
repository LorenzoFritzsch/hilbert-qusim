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

#ifndef COMPLEX_VECTOR_SPLIT_H
#define COMPLEX_VECTOR_SPLIT_H

#include "hilbert_namespace.h"
#include <stdexcept>
#include <vector>

#if defined(__APPLE__)
#include <Accelerate/Accelerate.h>
#else
#include <cstddef>
#include <immintrin.h>
#endif

class ComplexVectSplit {
public:
  ComplexVectSplit() = default;

  inline ComplexVectSplit(const std::vector<Complex> v) {
    for (auto k : v) {
      real_.push_back(k.real());
      imag_.push_back(k.imag());
    }
  }

  ComplexVectSplit(std::vector<__complex_precision> real,
                   std::vector<__complex_precision> imag)
      : real_(real), imag_(imag) {
    if (real_.size() != imag_.size()) {
      throw std::invalid_argument("Real and imag sizes do not match");
    }
  }

  [[nodiscard]] std::vector<__complex_precision> real() const { return real_; }

  [[nodiscard]] std::vector<__complex_precision> imag() const { return imag_; }

  [[nodiscard]] Complex get(const size_t i) const {
    return Complex(real_[i], imag_[i]);
  }

  [[nodiscard]] std::vector<Complex> get() const {
    std::vector<Complex> result(real_.size());
    for (size_t i = 0; i < real_.size(); i++) {
      result[i] = Complex(real_[i], imag_[i]);
    }
    return result;
  }

  [[nodiscard]] size_t size() const { return real_.size(); }

  void add(const Complex c) {
    real_.emplace_back(c.real());
    imag_.emplace_back(c.imag());
  }

  ComplexVectSplit conj() {
    __complex_precision k = -1;
    std::vector<__complex_precision> imag_conj(imag_.size());

#if defined(__APPLE__)
    vDSP_vsmul(imag_.data(), 1, &k, imag_conj.data(), 1, imag_conj.size());
#else
    __m256 scalar_vec = _mm256_set1_ps(k);

    size_t i = 0;
    size_t element_size = sizeof(__complex_precision) * 8;
    size_t elements_per_block = 256 / element_size;
    auto src = imag_.data();
    for (; i + elements_per_block <= imag_conj.size();
         i += elements_per_block) {
      __m256 vec = _mm256_loadu_ps(src + i);
      __m256 result = _mm256_mul_ps(vec, scalar_vec);
      _mm256_storeu_ps(imag_conj.data() + i, result);
    }
    for (; i < imag_conj.size(); i++) {
      imag_conj[i] = src[i] * k;
    }
#endif
    return ComplexVectSplit(real_, imag_conj);
  }

private:
  std::vector<__complex_precision> real_;
  std::vector<__complex_precision> imag_;
};

#endif // !COMPLEX_VECTOR_SPLIT_H
