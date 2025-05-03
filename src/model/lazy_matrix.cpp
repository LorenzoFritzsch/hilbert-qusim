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

// Recursive call chain broken once the underlying non-lazy element is reached.
Complex LazyMatrix::get(const int m, const int n) const {
  if ((*c_)[m][n] != std::nullopt) {
    return *(*c_)[m][n];
  }
  Complex v;
  switch (operation_) {
    case LazyMatrixOperation::TENSOR_PRODUCT: {
      const auto b_row_size = static_cast<int>(b_.value()->size());
      const auto b_col_size = static_cast<int>(b_.value()->at(0).size());
      const auto a_m = get_sector(m, b_row_size);
      const auto a_n = get_sector(n, b_col_size);
      const auto b_m = m % b_col_size;
      const auto b_n = n % b_col_size;
      v = (*a_)->at(a_m).at(a_n) * (*b_)->at(b_m).at(b_n);
      break;
    }
    case LazyMatrixOperation::LAZY_TENSOR_PRODUCT: {
      const auto b_row_size = (*b_lazy_)->size();
      const auto b_col_size = (*b_lazy_)->size();
      const auto a_m = get_sector(m, b_row_size);
      const auto a_n = get_sector(n, b_col_size);
      const auto b_m = m % b_col_size;
      const auto b_n = n % b_col_size;
      v = (*a_lazy_)->get(a_m, a_n) * (*b_lazy_)->get(b_m, b_n);
      break;
    }
    case LazyMatrixOperation::SCALAR_PRODUCT: {
      v = (*a_)->at(m).at(n) * k_.value();
      break;
    }
  }
  (*c_)[m][n] = v;
  return v;
}

ComplexVector LazyMatrix::get_row(const int m) const {
  auto result = ComplexVector((*c_)[m].size(), 0);
  for (int i = 0; i < (*c_)[m].size(); i++) {
    result[i] = get(m, i);
  }
  return result;
}

ComplexMatrix LazyMatrix::get(const bool complete) const {
  if (!complete) {
    return to_complex_matrix(*c_);
  }
  auto result = ComplexMatrix(c_->size(), ComplexVector(c_->size(), 0));
  for (int m = 0; m < size(); m++) {
    for (int n = 0; n < size(); n++) {
      result[m][n] = get(m, n);
    }
  }
  return result;
}

int LazyMatrix::size() const {
  return static_cast<int>(c_->size());
}