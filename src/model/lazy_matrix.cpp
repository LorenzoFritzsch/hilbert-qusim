#include "lazy_matrix.h"

int getSector(const int n, const int d) {
  int k = 0;
  while ((k + 1) * d <= n) {
    k++;
  }
  return k;
}

ComplexMatrix toComplexMatrix(const ComplexOptionalMatrix& optMatrix) {
  ComplexMatrix result;
  result.reserve(optMatrix.size());

  for (const auto& row : optMatrix) {
    ComplexVector newRow;
    newRow.reserve(row.size());
    for (const auto& elem : row) {
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
      const auto a_m = getSector(m, getBRowSize());
      const auto a_n = getSector(n, getBColSize());
      const auto b_m = m % getBColSize();
      const auto b_n = n % getBColSize();
      const auto v = getFromA(a_m, a_n) * getFromB(b_m, b_n);
      c[m][n] = v;
      return v;
    }
    case LazyMatrixOperation::SCALAR_PRODUCT: {
      const auto v = getFromA(m, n) * k.value();
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

// Recursion is broken once the value of the original ComplexMatrices are finally accessed
Complex LazyMatrix::getFromA(const int m, const int n) const {
  if (a) {
    return (*a)->at(m).at(n);
  }
  return (*aLazy)->get(m, n);
}

Complex LazyMatrix::getFromB(const int m, const int n) const {
  if (b) {
    auto asdf = *(b.value());
    return (*b)->at(m).at(n);
  }
  return (*bLazy)->get(m, n);
}

int LazyMatrix::getBRowSize() const {
  if (b) {
    return static_cast<int>(b.value()->size());
  }
  return (*bLazy)->getSize();
}

int LazyMatrix::getBColSize() const {
  if (b) {
    return static_cast<int>(b.value()->at(0).size());
  }
  return (*bLazy)->getSize();
}
