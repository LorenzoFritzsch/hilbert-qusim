#include "lazy_matrix.h"

int get_sector(const int n, const int d) {
  int k = 0;
  while ((k + 1) * d <= n) {
    k++;
  }
  return k;
}

ComplexMatrix to_complex_matrix(const ComplexOptionalMatrix& opt_matrix) {
  ComplexMatrix result;
  result.reserve(opt_matrix.size());

  for (const auto& row : opt_matrix) {
    ComplexVector new_row;
    new_row.reserve(row.size());
    for (const auto& elem : row) {
      new_row.push_back(elem.value_or(Complex(0)));
    }
    result.push_back(std::move(new_row));
  }
  return result;
}

Complex LazyMatrix::get(const int m, const int n) {
  if (c_[m][n] != std::nullopt) {
    return *c_[m][n];
  }
  switch (operation_) {
    case LazyMatrixOperation::TENSORIAL_PRODUCT: {
      const auto a_m = get_sector(m, get_b_row_size());
      const auto a_n = get_sector(n, get_b_col_size());
      const auto b_m = m % get_b_col_size();
      const auto b_n = n % get_b_col_size();
      const auto v = get_from_a(a_m, a_n) * get_from_b(b_m, b_n);
      c_[m][n] = v;
      return v;
    }
    case LazyMatrixOperation::SCALAR_PRODUCT: {
      const auto v = get_from_a(m, n) * k_.value();
      c_[m][n] = v;
      return v;
    }
    default: throw std::runtime_error("LazyMatrix::get: operation not recognized");
  }
}

ComplexVector LazyMatrix::get_row(const int m) {
  auto result = ComplexVector(c_[m].size(), 0);
  for (int i = 0; i < c_[m].size(); i++) {
    result[i] = get(m, i);
  }
  return result;
}

ComplexMatrix LazyMatrix::get(const bool complete) {
  if (!complete) {
    return to_complex_matrix(c_);
  }
  for (int m = 0; m < size(); m++) {
    for (int n = 0; n < size(); n++) {
      get(m, n);
    }
  }
  return to_complex_matrix(c_);
}

int LazyMatrix::size() const {
  return static_cast<int>(c_.size());
}

// Recursion is broken once the value of the original ComplexMatrices are finally accessed
Complex LazyMatrix::get_from_a(const int m, const int n) const {
  if (a_) {
    return (*a_)->at(m).at(n);
  }
  return (*a_lazy_)->get(m, n);
}

Complex LazyMatrix::get_from_b(const int m, const int n) const {
  if (b_) {
    return (*b_)->at(m).at(n);
  }
  return (*b_lazy_)->get(m, n);
}

int LazyMatrix::get_b_row_size() const {
  if (b_) {
    return static_cast<int>(b_.value()->size());
  }
  return (*b_lazy_)->size();
}

int LazyMatrix::get_b_col_size() const {
  if (b_) {
    return static_cast<int>(b_.value()->at(0).size());
  }
  return (*b_lazy_)->size();
}
