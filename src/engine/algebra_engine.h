#ifndef ALGEBRA_ENGINE_H
#define ALGEBRA_ENGINE_H

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"

#include <memory>

class AlgebraEngine {
public:
  AlgebraEngine() = delete;

  static std::unique_ptr<LazyOperation>
  conjugate_transpose(const ComplexVectMatrix &mat);

  static std::unique_ptr<LazyOperation>
  inner_product(const ComplexVectMatrix &vect_left,
                const ComplexVectMatrix &vect_right);

  static std::unique_ptr<LazyOperation>
  matrix_multiplication(const ComplexVectMatrix &mat_left,
                        const ComplexVectMatrix &mat_right);

  static std::unique_ptr<LazyOperation>
  matrix_vector_product(const ComplexVectMatrix &mat,
                        const ComplexVectMatrix &vect);

  static std::unique_ptr<LazyOperation>
  matrix_vector_product(std::unique_ptr<LazyOperation> mat,
                        const ComplexVectMatrix &vect);

  static std::unique_ptr<LazyOperation>
  matrix_vector_product(std::unique_ptr<LazyOperation> mat,
                        std::unique_ptr<LazyOperation> vect);

  static std::unique_ptr<LazyOperation>
  outer_product(const ComplexVectMatrix &mat_left,
                const ComplexVectMatrix &mat_right);

  static std::unique_ptr<LazyOperation>
  scalar_product(const ComplexVectMatrix &mat, const Complex &k);

  static std::unique_ptr<LazyOperation> sum(const ComplexVectMatrix &mat_left,
                                            const ComplexVectMatrix &mat_right);

  static std::unique_ptr<LazyOperation> sum(const ComplexVectMatrix &mat_left,
                                            const LazyOperation &mat_right);

  static std::unique_ptr<LazyOperation>
  tensor_product(const ComplexVectMatrix &mat_left,
                 const ComplexVectMatrix &mat_right);

  static std::unique_ptr<LazyOperation>
  tensor_product(const ComplexVectMatrix &mat, int times);

  static bool is_unitary(const ComplexVectMatrix &mat);
};

#endif // !ALGEBRA_ENGINE_H
