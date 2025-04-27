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
    : a_(std::move(a)),
      b_(std::move(b)),
      a_lazy_(std::nullopt),
      b_lazy_(std::nullopt),
      k_(std::nullopt),
      operation_(operation) {
    switch (operation) {
      case LazyMatrixOperation::TENSORIAL_PRODUCT: {
        const auto c_size = this->a_.value()->size() * this->b_.value()->size();
        c_ = ComplexOptionalMatrix(c_size, std::vector<std::optional<std::complex<double> > >(c_size, std::nullopt));
        break;
      }
      default: throw std::invalid_argument("LazyMatrix: invalid operation");
    }
  }

  LazyMatrix(std::shared_ptr<ComplexMatrix> m, const Complex k)
    : a_(std::move(m)), b_(std::nullopt),
      a_lazy_(std::nullopt),
      b_lazy_(std::nullopt), k_(k),
      operation_(LazyMatrixOperation::SCALAR_PRODUCT) {
    const auto c_size = this->a_.value()->size();
    c_ = ComplexOptionalMatrix(c_size, std::vector<std::optional<std::complex<double> > >(c_size, std::nullopt));
  }

  LazyMatrix(std::shared_ptr<LazyMatrix> a, std::shared_ptr<LazyMatrix> b, const LazyMatrixOperation operation)
    : a_(std::nullopt), b_(std::nullopt),
      a_lazy_(std::move(a)),
      b_lazy_(std::move(b)),
      k_(std::nullopt),
      operation_(operation) {
    switch (operation) {
      case LazyMatrixOperation::TENSORIAL_PRODUCT: {
        const auto c_size = (*a_lazy_)->size() * (*b_lazy_)->size();
        c_ = ComplexOptionalMatrix(c_size, std::vector<std::optional<std::complex<double> > >(c_size, std::nullopt));
        break;
      }
      default: throw std::invalid_argument("LazyMatrix: invalid operation");
    }
  }

  Complex get(int m, int n);

  ComplexVector get_row(int m);

  ComplexMatrix get(bool complete = true);

  [[nodiscard]] int size() const;

private:
  const std::optional<std::shared_ptr<ComplexMatrix>> a_;
  const std::optional<std::shared_ptr<ComplexMatrix>> b_;
  const std::optional<std::shared_ptr<LazyMatrix>> a_lazy_;
  const std::optional<std::shared_ptr<LazyMatrix>> b_lazy_;
  const std::optional<Complex> k_;
  const LazyMatrixOperation operation_;
  ComplexOptionalMatrix c_;

  [[nodiscard]] Complex get_from_a(int m, int n) const;

  [[nodiscard]] Complex get_from_b(int m, int n) const;

  [[nodiscard]] int get_b_row_size() const;

  [[nodiscard]] int get_b_col_size() const;
};

#endif // !LAZY_MATRIX_H
