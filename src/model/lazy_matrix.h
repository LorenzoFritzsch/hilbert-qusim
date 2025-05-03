#ifndef LAZY_MATRIX_H
#define LAZY_MATRIX_H

#include <memory>
#include "hilbert_namespace.h"

class LazyMatrix {
public:
  LazyMatrix(std::unique_ptr<ComplexMatrix> a, std::unique_ptr<ComplexMatrix> b)
    : a_(std::move(a)),
      b_(std::move(b)),
      a_lazy_(std::nullopt),
      b_lazy_(std::nullopt),
      k_(std::nullopt),
      operation_(LazyMatrixOperation::TENSOR_PRODUCT) {
    const auto c_size = this->a_.value()->size() * this->b_.value()->size();
    c_ = std::make_unique<ComplexOptionalMatrix>(
      c_size, std::vector<std::optional<std::complex<double> > >(c_size, std::nullopt));
  }

  LazyMatrix(std::unique_ptr<ComplexMatrix> m, const Complex k)
    : a_(std::move(m)), b_(std::nullopt),
      a_lazy_(std::nullopt),
      b_lazy_(std::nullopt), k_(k),
      operation_(LazyMatrixOperation::SCALAR_PRODUCT) {
    const auto c_size = this->a_.value()->size();
    c_ = std::make_unique<ComplexOptionalMatrix>(
      c_size, std::vector<std::optional<std::complex<double> > >(c_size, std::nullopt));
  }

  LazyMatrix(std::unique_ptr<LazyMatrix> a, std::unique_ptr<LazyMatrix> b)
    : a_(std::nullopt), b_(std::nullopt),
      a_lazy_(std::move(a)),
      b_lazy_(std::move(b)),
      k_(std::nullopt),
      operation_(LazyMatrixOperation::LAZY_TENSOR_PRODUCT) {
    const auto c_size = (*a_lazy_)->size() * (*b_lazy_)->size();
    c_ = std::make_unique<ComplexOptionalMatrix>(
      c_size, std::vector<std::optional<std::complex<double> > >(c_size, std::nullopt));
  }

  LazyMatrix(const LazyMatrix& other)
      : a_(other.a_ ? std::make_optional(std::make_unique<ComplexMatrix>(*other.a_.value())) : std::nullopt),
        b_(other.b_ ? std::make_optional(std::make_unique<ComplexMatrix>(*other.b_.value())) : std::nullopt),
        a_lazy_(other.a_lazy_ ? std::make_optional(std::make_unique<LazyMatrix>(*other.a_lazy_.value())) : std::nullopt),
        b_lazy_(other.b_lazy_ ? std::make_optional(std::make_unique<LazyMatrix>(*other.b_lazy_.value())) : std::nullopt),
        k_(other.k_),
        operation_(other.operation_),
        c_(std::make_unique<ComplexOptionalMatrix>(*other.c_)) {}

  [[nodiscard]] Complex get(int m, int n) const;

  [[nodiscard]] ComplexVector get_row(int m) const;

  [[nodiscard]] ComplexMatrix get(bool complete = true) const;

  [[nodiscard]] int size() const;

private:
  enum class LazyMatrixOperation {
    TENSOR_PRODUCT,
    LAZY_TENSOR_PRODUCT,
    SCALAR_PRODUCT
  };
  const std::optional<std::unique_ptr<ComplexMatrix>> a_;
  const std::optional<std::unique_ptr<ComplexMatrix>> b_;
  const std::optional<std::unique_ptr<LazyMatrix>> a_lazy_;
  const std::optional<std::unique_ptr<LazyMatrix>> b_lazy_;
  const std::optional<Complex> k_;
  const LazyMatrixOperation operation_;
  std::unique_ptr<ComplexOptionalMatrix> c_;
};

#endif // !LAZY_MATRIX_H
