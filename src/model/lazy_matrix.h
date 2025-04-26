#ifndef LAZY_MATRIX_H
#define LAZY_MATRIX_H

#include "hilbert_namespace.h"

enum class LazyMatrixOperation {
  TENSORIAL_PRODUCT,
  SCALAR_PRODUCT
};

class LazyMatrix {
public:
  LazyMatrix(std::shared_ptr<ComplexMatrix> a, std::shared_ptr<ComplexMatrix> b, const LazyMatrixOperation operation)
    : a(std::move(a)),
      b(std::move(b)),
      aLazy(std::nullopt),
      bLazy(std::nullopt),
      k(std::nullopt),
      operation(operation) {
    switch (operation) {
      case LazyMatrixOperation::TENSORIAL_PRODUCT: {
        const auto cSize = this->a.value()->size() * this->b.value()->size();
        c = ComplexOptionalMatrix(cSize, std::vector<std::optional<std::complex<double> > >(cSize, std::nullopt));
        break;
      }
      default: throw std::invalid_argument("LazyMatrix: invalid operation");
    }
  }

  LazyMatrix(std::shared_ptr<ComplexMatrix> m, const Complex k)
    : a(std::move(m)), b(std::nullopt),
      aLazy(std::nullopt),
      bLazy(std::nullopt), k(k),
      operation(LazyMatrixOperation::SCALAR_PRODUCT) {
    const auto cSize = this->a.value()->size();
    c = ComplexOptionalMatrix(cSize, std::vector<std::optional<std::complex<double> > >(cSize, std::nullopt));
  }

  LazyMatrix(std::shared_ptr<LazyMatrix> a, std::shared_ptr<LazyMatrix> b, const LazyMatrixOperation operation)
    : a(std::nullopt), b(std::nullopt),
      aLazy(std::move(a)),
      bLazy(std::move(b)),
      k(std::nullopt),
      operation(operation) {
    switch (operation) {
      case LazyMatrixOperation::TENSORIAL_PRODUCT: {
        const auto cSize = (*aLazy)->getSize() * (*bLazy)->getSize();
        c = ComplexOptionalMatrix(cSize, std::vector<std::optional<std::complex<double> > >(cSize, std::nullopt));
        break;
      }
      default: throw std::invalid_argument("LazyMatrix: invalid operation");
    }
  }

  Complex get(int m, int n);

  ComplexMatrix get(bool complete = true);

  [[nodiscard]] int getSize() const;

private:
  const std::optional<std::shared_ptr<ComplexMatrix>> a;
  const std::optional<std::shared_ptr<ComplexMatrix>> b;
  const std::optional<std::shared_ptr<LazyMatrix>> aLazy;
  const std::optional<std::shared_ptr<LazyMatrix>> bLazy;
  const std::optional<Complex> k;
  const LazyMatrixOperation operation;
  ComplexOptionalMatrix c;

  [[nodiscard]] Complex getFromA(int m, int n) const;

  [[nodiscard]] Complex getFromB(int m, int n) const;

  [[nodiscard]] int getBRowSize() const;

  [[nodiscard]] int getBColSize() const;
};

#endif // !LAZY_MATRIX_H
