#ifndef COMPLEX_VECTOR_SPLIT_H
#define COMPLEX_VECTOR_SPLIT_H

#include "hilbert_namespace.h"
#include <stdexcept>
#include <vector>

#include <Accelerate/Accelerate.h>

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

  [[nodiscard]] std::vector<Complex> get() const {
    std::vector<Complex> result(real_.size());
    for (int i = 0; i < real_.size(); i++) {
      result[i] = Complex(real_[i], imag_[i]);
    }
    return result;
  }

  [[nodiscard]] int size() const { return real_.size(); }

  void add(const Complex c) {
    real_.emplace_back(c.real());
    imag_.emplace_back(c.imag());
  }

  ComplexVectSplit conj() {
    __complex_precision k = -1;
    std::vector<__complex_precision> imag_conj(imag_.size());
    vDSP_vsmul(imag_.data(), 1, &k, imag_conj.data(), 1, imag_conj.size());
    return ComplexVectSplit(real_, imag_conj);
  }

private:
  std::vector<__complex_precision> real_;
  std::vector<__complex_precision> imag_;
};

#endif // !COMPLEX_VECTOR_SPLIT_H
