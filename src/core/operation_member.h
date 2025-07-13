#ifndef OP_MEMBER_H
#define OP_MEMBER_H

#include "hilbert_namespace.h"
#include <memory>

class OpMember {
public:
  virtual ~OpMember() = default;

  [[nodiscard]] virtual Complex get(int m, int n) const = 0;

  [[nodiscard]] virtual int row_size() const = 0;

  [[nodiscard]] virtual int column_size() const = 0;

  [[nodiscard]] virtual std::unique_ptr<OpMember> clone() const = 0;
};

#endif // !OP_MEMBER_H
