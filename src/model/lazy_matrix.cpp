#include "lazy_matrix.h"

int getSector(const int n, const int d) {
  int k = 0;
  while ((k + 1) * d <= n) {
    k++;
  }
  return k;
}

int getMatrixRowSize(const ComplexMatrix &m) {
  return static_cast<int>(m.size());
}

int getMatrixColumnSize(const ComplexMatrix &m) {
  return static_cast<int>(m[0].size());
}

ComplexMatrix toComplexMatrix(const ComplexOptionalMatrix& optMatrix) {
  ComplexMatrix result;
  result.reserve(optMatrix.size());

  for (const auto& row : optMatrix) {
    ComplexVector newRow;
    newRow.reserve(row.size());
    for (const auto& elem : row) {
      // If elem has a value, use it; otherwise, use Complex(0)
      newRow.push_back(elem.value_or(Complex(0)));
    }
    result.push_back(std::move(newRow));
  }
  return result;
}

Complex LazyMatrix::get(const int m, const int n) {
  if (c[m][n] != std::nullopt) {
    return *c[m][n];
  }
  switch (operation) {
    case LazyMatrixOperation::TENSORIAL_PRODUCT: {
      const auto a_m = getSector(m, getMatrixRowSize(b));
      const auto a_n = getSector(n, getMatrixColumnSize(b));
      const auto b_m = m % getMatrixColumnSize(b);
      const auto b_n = n % getMatrixColumnSize(b);
      const auto v = a[a_m][a_n] * b[b_m][b_n];
      c[m][n] = v;
      return v;
    }
    case LazyMatrixOperation::SCALAR_PRODUCT: {
      const auto v = a[m][n] * k;
      c[m][n] = v;
      return v;
    }
    default: throw std::runtime_error("LazyMatrix::get: operation not recognized");
  }
}

ComplexMatrix LazyMatrix::get(const bool complete) {
  if (!complete) {
    return toComplexMatrix(c);
  }
  for (int m = 0; m < getSize(); m++) {
    for (int n = 0; n < getSize(); n++) {
      get(m, n);
    }
  }
  return toComplexMatrix(c);
}

int LazyMatrix::getSize() const {
  return static_cast<int>(c.size());
}
