#include "lazy_vector.h"

Complex row_column_multiplication(const ComplexVector &m_v, const ComplexVector &v) {
  if (m_v.size() != v.size()) {
    throw std::invalid_argument("LazyVector: Vectors must have the same size");
  }
  Complex result(0, 0);
  for (int i = 0; i < m_v.size(); i++) {
    result += m_v[i] * v[i];
  }
  return result;
}

// Recursive call chain broken once the underlying non-lazy element is reached.
Complex LazyVector::get(const int i) const {
  if (c_->at(i) != std::nullopt) {
    return *c_->at(i);
  }
  Complex v;
  switch (operation_) {
    case LazyVectorOperation::MATRIX_VECTOR_PRODUCT:
      v = row_column_multiplication((*m_.value())[i], *v_.value());
      break;
    case LazyVectorOperation::LAZY_MATRIX_VECTOR_PRODUCT:
      v = row_column_multiplication(m_lazy_.value()->get_row(i), *v_.value());
      break;
    case LazyVectorOperation::LAZY_MATRIX_LAZY_VECTOR_PRODUCT:
      v = row_column_multiplication(m_lazy_.value()->get_row(i), v_lazy_.value()->get());
      break;
    case LazyVectorOperation::SCALAR_PRODUCT:
      v = k_.value() * (*v_.value())[i];
      break;
    case LazyVectorOperation::LAZY_SCALAR_PRODUCT:
      v = k_.value() * v_lazy_.value()->get(i);
      break;
  }
  c_->at(i) = v;
  return v;
}

ComplexVector LazyVector::get() const {
  auto result = ComplexVector(c_->size(), 0);
  for (int i = 0; i < c_->size(); i++) {
    result[i] = get(i);
  }
  return result;
}

int LazyVector::size() const {
  return static_cast<int>(c_->size());
}
