#ifndef LAZY_VECTOR_OPERATION_H
#define LAZY_VECTOR_OPERATION_H
#include "hilbert_namespace.h"

class LazyVectorOperation {
public:
  virtual ~LazyVectorOperation() = default;

  virtual Complex get(int i) = 0;

  virtual int size() = 0;

  [[nodiscard]] virtual std::unique_ptr<LazyVectorOperation> clone() const = 0;

protected:
  static int get_index(const int n, const int d) {
    int k = 0;
    while ((k + 1) * d <= n) {
      k++;
    }
    return k;
  }

  static Complex row_column_multiplication(const ComplexVector &m_v, const ComplexVector &v) {
    if (m_v.size() != v.size()) {
      throw std::invalid_argument("LazyVector: Vectors must have the same size");
    }
    Complex result(0, 0);
    for (int i = 0; i < m_v.size(); i++) {
      result += m_v[i] * v[i];
    }
    return result;
  }
};


#endif // !LAZY_VECTOR_OPERATION_H
