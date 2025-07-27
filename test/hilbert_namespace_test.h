#ifndef HILBERT_NAMESPACE_TEST_H
#define HILBERT_NAMESPACE_TEST_H

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include <iostream>

inline bool are_matrices_equal(const ComplexVectMatrix &left,
                               const ComplexVectMatrix &right) {
  if (left.row_size() != right.row_size() ||
      left.column_size() != right.column_size()) {
    return false;
  }
  for (int i = 0; i < left.row_size(); i++) {
    for (int j = 0; j < left.column_size(); j++) {
      if (left.get(i, j) != right.get(i, j)) {
        return false;
      }
    }
  }
  return true;
}

inline bool are_matrices_equal(const ComplexVectMatrix &left,
                               const LazyOperation &right) {
  if (left.row_size() != right.row_size() ||
      left.column_size() != right.column_size()) {
    return false;
  }
  for (int i = 0; i < left.row_size(); i++) {
    for (int j = 0; j < left.column_size(); j++) {
      if (left.get(i, j) != right.get(i, j)) {
        return false;
      }
    }
  }
  return true;
}

inline bool run_test(const std::string &title, std::function<bool()> test) {
  std::cout << title << std::string(60 - title.length(), '.');
  try {
    auto result = test();
    if (result) {
      std::cout << "\033[32m PASSED\033[0m";
    } else {
      std::cout << "\033[31m FAILED\033[0m";
    }
    std::cout << std::endl;
    return result;
  } catch (const std::exception &e) {
    std::cout << "\033[31m FAILED: " << e.what() << "\033[0m" << std::endl;
    return false;
  }
}

#endif // !HILBERT_NAMESPACE_TEST_H
