#ifndef LAZY_VECTOR_H
#define LAZY_VECTOR_H

#include <memory>
#include "lazy_matrix.h"

class LazyVector {
public:
  LazyVector(std::unique_ptr<ComplexMatrix> matrix, std::unique_ptr<ComplexVector> vector)
    : m_(std::move(matrix)), v_(std::move(vector)),
      m_lazy_(std::nullopt), v_lazy_(std::nullopt), k_(std::nullopt),
      operation_(LazyVectorOperation::MATRIX_VECTOR_PRODUCT) {
    if (m_.value()->size() != v_.value()->size()) {
      throw std::invalid_argument("LazyVector: matrix and vector sizes don't match");
    }
    c_ = std::make_unique<ComplexOptionalVector>(v_.value()->size(), std::nullopt);
  }

  LazyVector(std::unique_ptr<LazyMatrix> matrix, std::unique_ptr<ComplexVector> vector)
    : m_(std::nullopt),
      v_(std::move(vector)), m_lazy_(std::move(matrix)),
      v_lazy_(std::nullopt), k_(std::nullopt),
      operation_(LazyVectorOperation::LAZY_MATRIX_VECTOR_PRODUCT) {
    if (m_lazy_.value()->size() != v_.value()->size()) {
      throw std::invalid_argument("LazyVector: matrix and vector sizes don't match");
    }
    c_ = std::make_unique<ComplexOptionalVector>(v_.value()->size(), std::nullopt);
  }

  LazyVector(std::unique_ptr<LazyMatrix> matrix, std::unique_ptr<LazyVector> vector)
    : m_(std::nullopt), v_(std::nullopt),
      m_lazy_(std::move(matrix)), v_lazy_(std::move(vector)),
      k_(std::nullopt),
      operation_(LazyVectorOperation::LAZY_MATRIX_LAZY_VECTOR_PRODUCT) {
    if (m_lazy_.value()->size() != v_lazy_.value()->size()) {
      throw std::invalid_argument("LazyVector: matrix and vector sizes don't match");
    }
    c_ = std::make_unique<ComplexOptionalVector>(v_lazy_.value()->size(), std::nullopt);
  }

  LazyVector(std::unique_ptr<ComplexVector> vector, Complex k)
    : m_(std::nullopt),
      v_(std::move(vector)),
      m_lazy_(std::nullopt), v_lazy_(std::nullopt),
      k_(k), operation_(LazyVectorOperation::SCALAR_PRODUCT) {
    c_ = std::make_unique<ComplexOptionalVector>(v_.value()->size(), std::nullopt);
  }

  LazyVector(std::unique_ptr<LazyVector> vector, Complex k)
    : m_(std::nullopt), v_(std::nullopt),
      m_lazy_(std::nullopt),
      v_lazy_(std::move(vector)),
      k_(k), operation_(LazyVectorOperation::LAZY_SCALAR_PRODUCT) {
    c_ = std::make_unique<ComplexOptionalVector>(v_lazy_.value()->size(), std::nullopt);
  }

  [[nodiscard]] Complex get(int i) const;

  [[nodiscard]] ComplexVector get() const;

  [[nodiscard]] int size() const;

private:
  enum class LazyVectorOperation {
    MATRIX_VECTOR_PRODUCT,
    LAZY_MATRIX_VECTOR_PRODUCT,
    LAZY_MATRIX_LAZY_VECTOR_PRODUCT,
    SCALAR_PRODUCT,
    LAZY_SCALAR_PRODUCT
  };

  const std::optional<std::unique_ptr<ComplexMatrix>> m_;
  const std::optional<std::unique_ptr<ComplexVector>> v_;
  const std::optional<std::unique_ptr<LazyMatrix>> m_lazy_;
  const std::optional<std::unique_ptr<LazyVector>> v_lazy_;
  const std::optional<Complex> k_;
  const LazyVectorOperation operation_;
  std::unique_ptr<ComplexOptionalVector> c_;
};

#endif // !LAZY_VECTOR_H
