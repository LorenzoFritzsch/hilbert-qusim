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
#include <memory>
#include <stdexcept>
#include <vector>

#ifdef __APPLE__
#include "simd_apple.h"
using __simd__ = simd_apple;
#else
#include "simd_avx.h"
using __simd__ = simd_avx;
#endif

class ComplexVectSplit {
public:
  ComplexVectSplit() = default;

  explicit ComplexVectSplit(const std::vector<Complex> v) {
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

  [[nodiscard]] std::unique_ptr<std::vector<__complex_precision>> real() const {
    return std::make_unique<std::vector<__complex_precision>>(real_);
  }

  [[nodiscard]] std::unique_ptr<std::vector<__complex_precision>> imag() const {
    return std::make_unique<std::vector<__complex_precision>>(imag_);
  }

  [[nodiscard]] Complex get(const size_t i) const {
    return Complex(real_[i], imag_[i]);
  }

  [[nodiscard]] std::unique_ptr<std::vector<Complex>> get() const {
    std::vector<Complex> result(real_.size());
    for (size_t i = 0; i < real_.size(); i++) {
      result[i] = Complex(real_[i], imag_[i]);
    }
    return std::make_unique<std::vector<Complex>>(result);
  }

  [[nodiscard]] size_t size() const { return real_.size(); }

  void add(const Complex c) {
    real_.emplace_back(c.real());
    imag_.emplace_back(c.imag());
  }

  std::unique_ptr<ComplexVectSplit> conj() {
    __complex_precision k = -1;
    std::vector<__complex_precision> imag_conj(imag_.size());
    __simd__::vsmul(imag_.data(), &k, imag_conj.data(), imag_conj.size());
    return std::make_unique<ComplexVectSplit>(real_, imag_conj);
  }

private:
  std::vector<__complex_precision> real_;
  std::vector<__complex_precision> imag_;
};

#endif // !COMPLEX_VECTOR_SPLIT_H
