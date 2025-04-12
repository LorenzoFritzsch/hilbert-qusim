#ifndef LAZY_MATRIX_H
#define LAZY_MATRIX_H

#include "hilbert_namespace.h"

enum class LazyMatrixOperation {
  TENSORIAL_PRODUCT,
  SCALAR_PRODUCT
};

class LazyMatrix {
public:
  LazyMatrix(ComplexMatrix a, ComplexMatrix b, const LazyMatrixOperation operation)
      : a(std::move(a)),
        b(std::move(b)),
        operation(operation) {
    switch (operation) {
      case LazyMatrixOperation::TENSORIAL_PRODUCT: {
        const auto cSize = this->a.size() * this->b.size();
        c = ComplexOptionalMatrix(cSize, std::vector<std::optional<std::complex<double> > >(cSize, std::nullopt));
        break;
      }
      default: throw std::invalid_argument("LazyMatrix: invalid operation");
    }
  }

  LazyMatrix(ComplexMatrix m, const Complex k) : a(std::move(m)), k(k), operation(LazyMatrixOperation::SCALAR_PRODUCT) {
    const auto cSize = this->a.size();
    c = ComplexOptionalMatrix(cSize, std::vector<std::optional<std::complex<double> > >(cSize, std::nullopt));
  }

  Complex get(int m, int n);

  ComplexMatrix get(bool complete = true);

  [[nodiscard]] int getSize() const;

private:
  const ComplexMatrix a;
  const ComplexMatrix b;
  const Complex k;
  const LazyMatrixOperation operation;
  ComplexOptionalMatrix c;
};

#endif // !LAZY_MATRIX_H
