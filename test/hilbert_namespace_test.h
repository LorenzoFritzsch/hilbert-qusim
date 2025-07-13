#ifndef HILBERT_NAMESPACE_TEST_H
#define HILBERT_NAMESPACE_TEST_H

#include "hilbert_namespace.h"
#include "operation_member.h"

inline bool are_matrices_equal(const OpMember &left, const OpMember &right) {
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

#endif // !HILBERT_NAMESPACE_TEST_H
