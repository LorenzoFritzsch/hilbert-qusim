#include "algebra_engine.h"

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include "operation_member.h"
#include <algorithm>
#include <memory>
#include <utility>

/*
 * Conjugate transpose
 */
Complex conjugate_transpose_lazy(const std::unique_ptr<OpMember> &left,
                                 const std::unique_ptr<OpMember> &right,
                                 const int m, const int n) {
  return std::conj(left->get(n, m));
}

/*
 * Inner product
 */
Complex inner_product_lazy(const std::unique_ptr<OpMember> &left,
                           const std::unique_ptr<OpMember> &right, const int m,
                           const int n) {
  Complex result = 0;
  const auto max = std::min(left->column_size(), right->column_size());
  for (int i = 0; i < max; i++) {
    result += left->get(0, i) * right->get(0, i);
  }
  return result;
}

/*
 * Row-column multiplication
 */
Complex matrix_multiplication_lazy(const std::unique_ptr<OpMember> &left,
                                   const std::unique_ptr<OpMember> &right,
                                   const int m, const int n) {
  Complex result(0, 0);
  for (int i = 0; i < left->column_size(); i++) {
    result += left->get(n, i) * right->get(m, i);
  }
  return result;
}

int matrix_multiplication_final_row_size(const OpMember &left,
                                         const OpMember &right) {
  return left.row_size();
}

int matrix_multiplication_final_column_size(const OpMember &left,
                                            const OpMember &right) {
  return right.column_size();
}

/*
 * Outer product
 */
Complex outer_product_lazy(const std::unique_ptr<OpMember> &left,
                           const std::unique_ptr<OpMember> &right, const int m,
                           const int n) {
  return left->get(0, m) * right->get(0, n);
}

/*
 * Scalar Product
 */
Complex scalar_product_lazy(const std::unique_ptr<OpMember> &left,
                            const std::unique_ptr<OpMember> &right, const int m,
                            const int n) {
  return left->get(m, n) * right->get(0, 0);
}

/*
 * Sum
 */
Complex sum_lazy(const std::unique_ptr<OpMember> &left,
                 const std::unique_ptr<OpMember> &right, const int m,
                 const int n) {
  return left->get(m, n) + right->get(m, n);
}

/*
 * Tensor product
 */
Complex tensor_product_lazy(const std::unique_ptr<OpMember> &left,
                            const std::unique_ptr<OpMember> &right, const int m,
                            const int n) {
  const auto a_val = left->get(m / right->row_size(), n / right->column_size());
  const auto b_val =
      right->get(m % right->row_size(), n % right->column_size());
  return a_val * b_val;
}

int tensor_product_final_row_size(const OpMember &left, const OpMember &right) {
  return left.row_size() * right.row_size();
}

int tensor_product_final_column_size(const OpMember &left,
                                     const OpMember &right) {
  return left.column_size() * right.column_size();
}

std::unique_ptr<LazyOperation>
AlgebraEngine::conjugate_transpose(std::unique_ptr<OpMember> mat) {
  int final_row_size = mat->column_size();
  int final_column_size = mat->row_size();
  return std::make_unique<LazyOperation>(
      std::move(mat), std::make_unique<ComplexVectMatrix>(),
      conjugate_transpose_lazy, final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::inner_product(std::unique_ptr<OpMember> vect_left,
                             std::unique_ptr<OpMember> vect_right) {
  if (vect_left->row_size() != 1 || vect_right->row_size() != 1) {
    throw std::invalid_argument("Input must be vectors");
  }
  return std::make_unique<LazyOperation>(
      std::move(vect_left), std::move(vect_right), inner_product_lazy, 1, 1);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_multiplication(std::unique_ptr<OpMember> mat_left,
                                     std::unique_ptr<OpMember> mat_right) {
  int final_row_size = mat_left->row_size();
  int final_column_size = mat_right->column_size();
  return std::make_unique<LazyOperation>(
      std::move(mat_left), std::move(mat_right), matrix_multiplication_lazy,
      final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(std::unique_ptr<OpMember> mat,
                                     std::unique_ptr<OpMember> vect) {
  int final_column_size = vect->column_size();
  return std::make_unique<LazyOperation>(std::move(mat), std::move(vect),
                                         matrix_multiplication_lazy, 1,
                                         final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::outer_product(std::unique_ptr<OpMember> mat_left,
                             std::unique_ptr<OpMember> mat_right) {
  int final_row_size = mat_left->column_size();
  int final_column_size = mat_right->column_size();
  return std::make_unique<LazyOperation>(
      std::move(mat_left), std::move(mat_right), outer_product_lazy,
      final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::scalar_product(std::unique_ptr<OpMember> mat, const Complex &k) {
  int final_row_size = mat->row_size();
  int final_column_size = mat->column_size();
  return std::make_unique<LazyOperation>(
      std::move(mat), std::make_unique<ComplexVectMatrix>(k),
      scalar_product_lazy, final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::sum(std::unique_ptr<OpMember> mat_left,
                   std::unique_ptr<OpMember> mat_right) {
  int mat_left_row_size = mat_left->row_size();
  int mat_right_column_size = mat_right->column_size();
  if (mat_left_row_size != mat_right->row_size() ||
      mat_left->column_size() != mat_right_column_size) {
    throw std::invalid_argument("Matrix sizes do not match");
  }
  return std::make_unique<LazyOperation>(
      std::move(mat_left), std::move(mat_right), sum_lazy, mat_left_row_size,
      mat_right_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(std::unique_ptr<OpMember> mat_left,
                              std::unique_ptr<OpMember> mat_right) {
  int final_row_size = mat_left->row_size() * mat_right->row_size();
  int final_column_size = mat_left->column_size() * mat_right->column_size();
  return std::make_unique<LazyOperation>(
      std::move(mat_left), std::move(mat_right), tensor_product_lazy,
      final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(std::unique_ptr<OpMember> mat, const int times) {
  if (times == 1) {
    return std::make_unique<LazyOperation>(std::move(mat));
  }

  int final_row_size = tensor_product_final_row_size(*mat, *mat);
  int final_column_size = tensor_product_final_column_size(*mat, *mat);

  auto lazy = std::make_unique<LazyOperation>(
      mat->clone(), mat->clone(), tensor_product_lazy, final_row_size,
      final_column_size);

  for (int _ = 2; _ < times; _++) {
    lazy->push(std::make_unique<LazyOperation>(mat->clone()),
               tensor_product_lazy, tensor_product_final_row_size,
               tensor_product_final_column_size);
  }
  return lazy;
}

bool AlgebraEngine::is_unitary(const OpMember &mat) {
  if (mat.row_size() != mat.column_size()) {
    return false;
  }

  auto a_dagger = AlgebraEngine::conjugate_transpose(mat.clone());
  int final_row_size = matrix_multiplication_final_row_size(mat, *a_dagger);
  int final_column_size =
      matrix_multiplication_final_column_size(mat, *a_dagger);
  auto lazy = std::make_unique<LazyOperation>(
      mat.clone(), std::move(a_dagger), matrix_multiplication_lazy,
      final_row_size, final_column_size);

  // TODO: Improve with multi-threading if fat matrix?
  // lazy should be an identity matrix
  for (int m = 0; m < lazy->row_size(); m++) {
    for (int n = 0; n < lazy->column_size(); n++) {
      if (m == n) {
        if (!approx_equal(lazy->get(m, n), Complex(1, 0))) {
          return false;
        }
      } else {
        if (!approx_equal(lazy->get(m, n), Complex(0, 0))) {
          return false;
        }
      }
    }
  }
  return true;
}
