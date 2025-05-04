#ifndef LAZY_MATRIX_OPERATION_H
#define LAZY_MATRIX_OPERATION_H
#include <memory>
#include "hilbert_namespace.h"

class LazyMatrixOperation {
public:
  virtual ~LazyMatrixOperation() = default;

  virtual Complex get(int m, int n) = 0;

  virtual int c_row_size() = 0;

  virtual int c_column_size() = 0;

  [[nodiscard]] virtual std::unique_ptr<LazyMatrixOperation> clone() const = 0;

protected:
  static int get_index(const int n, const int d) {
    int k = 0;
    while ((k + 1) * d <= n) {
      k++;
    }
    return k;
  }
};

#endif // !LAZY_MATRIX_OPERATION_H
