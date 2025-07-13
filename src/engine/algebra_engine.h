#ifndef ALGEBRA_ENGINE_H
#define ALGEBRA_ENGINE_H

#include <memory>

#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include "operation_member.h"

class AlgebraEngine {
public:
  AlgebraEngine() = delete;

  static std::unique_ptr<LazyOperation>
  conjugate_transpose(std::unique_ptr<OpMember> mat);

  static std::unique_ptr<LazyOperation>
  inner_product(std::unique_ptr<OpMember> vect_left,
                std::unique_ptr<OpMember> vect_right);

  static std::unique_ptr<LazyOperation>
  matrix_multiplication(std::unique_ptr<OpMember> mat_left,
                        std::unique_ptr<OpMember> mat_right);

  static std::unique_ptr<LazyOperation>
  matrix_vector_product(std::unique_ptr<OpMember> mat,
                        std::unique_ptr<OpMember> vect);

  static std::unique_ptr<LazyOperation>
  outer_product(std::unique_ptr<OpMember> mat_left,
                std::unique_ptr<OpMember> mat_right);

  static std::unique_ptr<LazyOperation>
  scalar_product(std::unique_ptr<OpMember> mat, const Complex &k);

  static std::unique_ptr<LazyOperation>
  sum(std::unique_ptr<OpMember> mat_left, std::unique_ptr<OpMember> mat_right);

  static std::unique_ptr<LazyOperation>
  tensor_product(std::unique_ptr<OpMember> mat_left,
                 std::unique_ptr<OpMember> mat_right);

  static std::unique_ptr<LazyOperation>
  tensor_product(std::unique_ptr<OpMember> mat, int times);

  static bool is_unitary(const OpMember &mat);
};

#endif // !ALGEBRA_ENGINE_H
