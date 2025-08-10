#include "algebra_engine.h"

#include "complex_vector_split.h"
#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include "operation.h"
#include <complex>
#include <memory>

#if defined(__APPLE__)
#include <Accelerate/Accelerate.h>
#endif

/*
 * SIMD element-wise complex vector multiplication
 */
ComplexVectSplit cvmul(const ComplexVectSplit &left,
                       const ComplexVectSplit &right) {

  // Considering the formula (a + bi)(c + di) = (ac - bd) + i(ad + bc):
  std::vector<__complex_precision> ac_vect(left.size()), bd_vect(left.size()),
      ad_vect(left.size()), bc_vect(left.size());

  std::vector<__complex_precision> result_real(left.size());
  std::vector<__complex_precision> result_imag(left.size());

  auto left_real = left.real();
  auto left_imag = left.imag();
  auto right_real = right.real();
  auto right_imag = right.imag();

#if defined(__APPLE__)
  vDSP_vmul(left_real.data(), 1, right_real.data(), 1, ac_vect.data(), 1,
            ac_vect.size());
  vDSP_vmul(left_imag.data(), 1, right_imag.data(), 1, bd_vect.data(), 1,
            bd_vect.size());
  vDSP_vmul(left_real.data(), 1, right_imag.data(), 1, ad_vect.data(), 1,
            ad_vect.size());
  vDSP_vmul(left_imag.data(), 1, right_real.data(), 1, bc_vect.data(), 1,
            bc_vect.size());

  vDSP_vsub(bd_vect.data(), 1, ac_vect.data(), 1, result_real.data(), 1,
            result_real.size());
  vDSP_vadd(ad_vect.data(), 1, bc_vect.data(), 1, result_imag.data(), 1,
            result_imag.size());
#endif

  return ComplexVectSplit(result_real, result_imag);
}

/*
 * SIMD sum over each element of the vector.
 */
Complex cvsve(const ComplexVectSplit &vect) {
  __complex_precision result_real = 0;
  __complex_precision result_imag = 0;
  auto vect_size = vect.size();

#if defined(__APPLE__)
  vDSP_sve(vect.real().data(), 1, &result_real, vect_size);
  vDSP_sve(vect.imag().data(), 1, &result_imag, vect_size);
#endif

  return Complex(result_real, result_imag);
}

/*
 * SIMD element-wise sum of two vectors.
 */
ComplexVectSplit cvadd(const ComplexVectSplit &left,
                       const ComplexVectSplit &right) {
  std::vector<__complex_precision> result_real(left.size()),
      result_imag(left.size());

#if defined(__APPLE__)
  vDSP_vadd(left.real().data(), 1, right.real().data(), 1, result_real.data(),
            1, result_real.size());
  vDSP_vadd(left.imag().data(), 1, right.imag().data(), 1, result_imag.data(),
            1, result_imag.size());
#endif

  return ComplexVectSplit(result_real, result_imag);
}

/*
 * SIMD scalar product.
 */
ComplexVectSplit cvsmul(const ComplexVectSplit &vect, const Complex &k) {
  auto vect_real = vect.real();
  auto vect_imag = vect.imag();
  auto k_real = static_cast<__complex_precision>(k.real());
  auto k_imag = static_cast<__complex_precision>(k.imag());

  // Considering the formula (a + bi)(c + di) = (ac - bd) + i(ad + bc):
  std::vector<__complex_precision> vect_ac(vect.size()), vect_bd(vect.size()),
      vect_ad(vect.size()), vect_bc(vect.size());

  std::vector<__complex_precision> result_real(vect.size()),
      result_imag(vect.size());

#if defined(__APPLE__)
  vDSP_vsmul(vect_real.data(), 1, &k_real, vect_ac.data(), 1, vect_ac.size());
  vDSP_vsmul(vect_imag.data(), 1, &k_imag, vect_bd.data(), 1, vect_bd.size());
  vDSP_vsmul(vect_real.data(), 1, &k_imag, vect_ad.data(), 1, vect_ad.size());
  vDSP_vsmul(vect_imag.data(), 1, &k_real, vect_bc.data(), 1, vect_bc.size());

  vDSP_vsub(vect_bd.data(), 1, vect_ac.data(), 1, result_real.data(), 1,
            result_real.size());
  vDSP_vadd(vect_ad.data(), 1, vect_bc.data(), 1, result_imag.data(), 1,
            result_imag.size());
#endif

  return ComplexVectSplit(result_real, result_imag);
}

/*
 * Conjugate transpose
 */
Complex conjugate_transpose_lazy(const ComplexVectMatrix &left,
                                 const ComplexVectMatrix &right, const int m,
                                 const int n) {
  return std::conj(left.get(n, m));
}

ComplexVectSplit conjugate_transpose_row(const ComplexVectMatrix &left,
                                         const ComplexVectMatrix &right,
                                         const int row) {
  return left.get_column(row).conj();
}

/*
 * Inner product
 */
Complex inner_product_mat_mat(const ComplexVectMatrix &left,
                              const ComplexVectMatrix &right, const int m,
                              const int n) {

  auto left_split = left.split().conj();
  auto right_split = right.split();

  auto elements = cvmul(left_split, right_split);

  __complex_precision result_real = 0;
  __complex_precision result_imag = 0;

  return cvsve(elements);
}

ComplexVectSplit inner_product_mat_mat_row(const ComplexVectMatrix &left,
                                           const ComplexVectMatrix &right,
                                           const int row) {
  ComplexVectSplit result;
  result.add(inner_product_mat_mat(left, right, 0, 0));
  return result;
}

/*
 * Row-column multiplication
 */
Complex matrix_multiplication_op_op(const Operation &left,
                                    const Operation &right, const int row,
                                    const int col) {
  auto vect_left = left.get(row);
  ComplexVectSplit vect_right;
  for (int m = 0; m < right.row_size(); m++) {
    vect_right.add(right.get(m, col));
  }

  auto elements = cvmul(vect_left, vect_right);

  return cvsve(elements);
}

ComplexVectSplit matrix_multiplication_op_op_row(const Operation &left,
                                                 const Operation &right,
                                                 const int row) {
  ComplexVectSplit result;
  for (int n = 0; n < right.row_size(); n++) {
    result.add(matrix_multiplication_op_op(left, right, row, n));
  }
  return result;
}

Complex matrix_multiplication_mat_mat(const ComplexVectMatrix &left,
                                      const ComplexVectMatrix &right,
                                      const int row, const int col) {
  auto vect_left = left.get_row(row);
  ComplexVectSplit vect_right;
  for (int m = 0; m < right.row_size(); m++) {
    vect_right.add(right.get(m, col));
  }

  auto elements = cvmul(vect_left, vect_right);

  return cvsve(elements);
}

ComplexVectSplit
matrix_multiplication_mat_mat_row(const ComplexVectMatrix &left,
                                  const ComplexVectMatrix &right,
                                  const int row) {
  auto row_left = left.get_row(row);
  ComplexVectSplit result;
  for (int n = 0; n < right.column_size(); n++) {
    auto column_right = right.get_column(n);
    auto elements = cvmul(row_left, column_right);
    result.add(cvsve(elements));
  }
  return result;
}

Complex matrix_multiplication_op_mat(const Operation &left,
                                     const ComplexVectMatrix &right,
                                     const int row, const int col) {
  auto vect_left = left.get(row);
  auto vect_right = right.get_column(col);

  auto elements = cvmul(vect_left, vect_right);

  return cvsve(elements);
}

ComplexVectSplit matrix_multiplication_op_mat_row(
    const Operation &left, const ComplexVectMatrix &right, const int row) {
  auto row_left = left.get(row);
  ComplexVectSplit result;
  for (int n = 0; n < right.column_size(); n++) {
    auto column_right = right.get_column(n);
    auto elements = cvmul(row_left, column_right);
    result.add(cvsve(elements));
  }
  return result;
}

Complex matrix_vector_mul_mat_mat(const ComplexVectMatrix &left,
                                  const ComplexVectMatrix &right, const int row,
                                  const int col) {
  auto vect_left = left.get_row(col);
  auto vect_right = right.split();
  auto elements = cvmul(vect_left, vect_right);
  return cvsve(elements);
}

ComplexVectSplit matrix_vector_mul_mat_mat_row(const ComplexVectMatrix &left,
                                               const ComplexVectMatrix &right,
                                               const int row) {
  ComplexVectSplit result;
  for (int col = 0; col < right.column_size(); col++) {
    result.add(matrix_vector_mul_mat_mat(left, right, row, col));
  }
  return result;
}

Complex matrix_vector_mul_op_op(const Operation &left, const Operation &right,
                                const int row, const int col) {
  auto vect_left = left.get(col);
  ComplexVectSplit vect_right;
  for (int m = 0; m < right.column_size(); m++) {
    vect_right.add(right.get(0, m));
  }

  auto elements = cvmul(vect_left, vect_right);

  return cvsve(elements);
}

ComplexVectSplit matrix_vector_mul_op_op_row(const Operation &left,
                                             const Operation &right,
                                             const int row) {
  ComplexVectSplit result;
  for (int col = 0; col < right.column_size(); col++) {
    result.add(matrix_vector_mul_op_op(left, right, row, col));
  }
  return result;
}

int matrix_multiplication_final_row_size(int row_size_left,
                                         int column_size_left,
                                         int row_size_right,
                                         int column_size_right) {
  return row_size_left;
}

int matrix_multiplication_final_column_size(int row_size_left,
                                            int column_size_left,
                                            int row_size_right,
                                            int column_size_right) {
  return column_size_right;
}

/*
 * Outer product
 */
Complex outer_product_mat_mat(const ComplexVectMatrix &left,
                              const ComplexVectMatrix &right, const int m,
                              const int n) {
  return left.get(0, m) * std::conj(right.get(0, n));
}

ComplexVectSplit outer_product_mat_mat_row(const ComplexVectMatrix &left,
                                           const ComplexVectMatrix &right,
                                           const int row) {
  auto vect_left = left.get_row(0);
  auto vect_right = right.get_row(0).conj();

  return cvmul(vect_left, vect_right);
}

/*
 * Scalar Product
 */
Complex scalar_product_mat_mat(const ComplexVectMatrix &left,
                               const ComplexVectMatrix &right, const int m,
                               const int n) {
  return left.get(m, n) * right.get(0, 0);
}

ComplexVectSplit scalar_product_mat_mat_row(const ComplexVectMatrix &left,
                                            const ComplexVectMatrix &right,
                                            const int row) {
  auto vect_left = left.get_row(row);
  auto k = right.get(0, 0);
  return cvsmul(vect_left, k);
}

/*
 * Sum
 */
Complex sum_mat_mat(const ComplexVectMatrix &left,
                    const ComplexVectMatrix &right, const int m, const int n) {
  return left.get(m, n) + right.get(m, n);
}

ComplexVectSplit sum_mat_mat_row(const ComplexVectMatrix &left,
                                 const ComplexVectMatrix &right,
                                 const int row) {
  ComplexVectSplit vect_left = left.get_row(row);
  ComplexVectSplit vect_right = right.get_row(row);

  return cvadd(vect_left, vect_right);
}

Complex sum_op_op(const Operation &left, const Operation &right, const int m,
                  const int n) {
  return left.get(m, n) + right.get(m, n);
}

ComplexVectSplit sum_op_op_row(const Operation &left, const Operation &right,
                               const int row) {
  ComplexVectSplit vect_left = left.get(row);
  ComplexVectSplit vect_right = right.get(row);

  return cvadd(vect_left, vect_right);
}

int sum_row_size(const int left_row_size, const int left_column_size,
                 const int right_row_size, const int right_column_size) {
  return left_row_size;
}

int sum_column_size(const int left_row_size, const int left_column_size,
                    const int right_row_size, const int right_column_size) {
  return left_column_size;
}

/*
 * Tensor product
 */
Complex tensor_product_mat_mat(const ComplexVectMatrix &left,
                               const ComplexVectMatrix &right, const int m,
                               const int n) {
  const auto a_val = left.get(m / right.row_size(), n / right.column_size());
  const auto b_val = right.get(m % right.row_size(), n % right.column_size());
  return a_val * b_val;
}

Complex tensor_product_op_mat(const Operation &left,
                              const ComplexVectMatrix &right, const int m,
                              const int n) {
  const auto a_val = left.get(m / right.row_size(), n / right.column_size());
  const auto b_val = right.get(m % right.row_size(), n % right.column_size());
  return a_val * b_val;
}

/*
 * Tensor product left-hand elements, given the left-hand matrix of the
 * operation and the range.
 */
ComplexVectSplit tplhe(const ComplexVectMatrix &left, const int row,
                       const int row_size, const int right_row_size,
                       const int right_column_size) {
  return left.get(
      row_size,
      [row, right_row_size](int i) -> int { return row / right_row_size; },
      [right_column_size](int i) -> int { return i / right_column_size; });
}

ComplexVectSplit tplhe(const Operation &left, const int row, const int row_size,
                       const int right_row_size, const int right_column_size) {
  ComplexVectSplit result;
  int m = row / right_row_size;
  auto left_row = left.get(m);
  for (int n = 0; n < row_size; n++) {
    result.add(left_row.get(n / right_column_size));
  }
  return result;
}

ComplexVectSplit tprhe(const ComplexVectMatrix &right, const int row,
                       const int row_size, const int right_row_size,
                       const int right_column_size) {
  return right.get(
      row_size,
      [row, right_row_size](int i) -> int { return row % right_row_size; },
      [right_column_size](int i) -> int { return i % right_column_size; });
}

ComplexVectSplit tensor_product_mat_mat_row(const ComplexVectMatrix &left,
                                            const ComplexVectMatrix &right,
                                            const int row) {
  ComplexVectSplit vect_left, vect_right;
  const auto right_row_size = right.row_size();
  const auto right_column_size = right.column_size();
  const auto final_row_size = left.row_size() * right_row_size;

  vect_left =
      tplhe(left, row, final_row_size, right_row_size, right_column_size);
  vect_right =
      tprhe(right, row, final_row_size, right_row_size, right_column_size);

  return cvmul(vect_left, vect_right);
}

ComplexVectSplit tensor_product_op_mat_row(const Operation &left,
                                           const ComplexVectMatrix &right,
                                           const int row) {
  ComplexVectSplit vect_left, vect_right;
  const auto right_row_size = right.row_size();
  const auto right_column_size = right.column_size();
  const auto final_row_size = left.row_size() * right_row_size;

  vect_left =
      tplhe(left, row, final_row_size, right_row_size, right_column_size);
  vect_right =
      tprhe(right, row, final_row_size, right_row_size, right_column_size);

  return cvmul(vect_left, vect_right);
}

int tensor_product_final_row_size(int row_size_left, int column_size_left,
                                  int row_size_right, int column_size_right) {
  return row_size_left * row_size_right;
}

int tensor_product_final_column_size(int row_size_left, int column_size_left,
                                     int row_size_right,
                                     int column_size_right) {
  return column_size_left * column_size_right;
}

std::unique_ptr<LazyOperation>
AlgebraEngine::conjugate_transpose(const ComplexVectMatrix &mat) {
  int final_row_size = mat.column_size();
  int final_column_size = mat.row_size();
  return std::make_unique<LazyOperation>(
      mat, ComplexVectMatrix(), conjugate_transpose_lazy,
      conjugate_transpose_row, final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::inner_product(const ComplexVectMatrix &vect_left,
                             const ComplexVectMatrix &vect_right) {
  if (vect_left.row_size() != 1 || vect_right.row_size() != 1) {
    throw std::invalid_argument("Input must be vectors");
  }
  return std::make_unique<LazyOperation>(vect_left, std::move(vect_right),
                                         inner_product_mat_mat,
                                         inner_product_mat_mat_row, 1, 1);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_multiplication(const ComplexVectMatrix &mat_left,
                                     const ComplexVectMatrix &mat_right) {
  int final_row_size = mat_left.row_size();
  int final_column_size = mat_right.column_size();
  return std::make_unique<LazyOperation>(
      mat_left, mat_right, matrix_multiplication_mat_mat,
      matrix_multiplication_mat_mat_row, final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(const ComplexVectMatrix &mat,
                                     const ComplexVectMatrix &vect) {
  int final_column_size = vect.column_size();
  return std::make_unique<LazyOperation>(mat, vect, matrix_vector_mul_mat_mat,
                                         matrix_vector_mul_mat_mat_row, 1,
                                         final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(std::unique_ptr<LazyOperation> mat,
                                     const ComplexVectMatrix &vect) {
  auto op = std::move(mat);
  op->append(vect, matrix_multiplication_op_mat,
             matrix_multiplication_op_mat_row,
             matrix_multiplication_final_row_size,
             matrix_multiplication_final_column_size);

  return std::move(op);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::matrix_vector_product(std::unique_ptr<LazyOperation> mat,
                                     std::unique_ptr<LazyOperation> vect) {
  auto op = std::move(mat);
  op->append(*std::move(vect), matrix_vector_mul_op_op,
             matrix_vector_mul_op_op_row, matrix_multiplication_final_row_size,
             matrix_multiplication_final_column_size);

  return std::move(op);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::outer_product(const ComplexVectMatrix &mat_left,
                             const ComplexVectMatrix &mat_right) {
  int final_row_size = mat_left.column_size();
  int final_column_size = mat_right.column_size();
  return std::make_unique<LazyOperation>(
      mat_left, mat_right, outer_product_mat_mat, outer_product_mat_mat_row,
      final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::scalar_product(const ComplexVectMatrix &mat, const Complex &k) {
  int final_row_size = mat.row_size();
  int final_column_size = mat.column_size();
  return std::make_unique<LazyOperation>(
      mat, ComplexVectMatrix(k), scalar_product_mat_mat,
      scalar_product_mat_mat_row, final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::sum(const ComplexVectMatrix &mat_left,
                   const ComplexVectMatrix &mat_right) {
  int mat_left_row_size = mat_left.row_size();
  int mat_right_column_size = mat_right.column_size();
  if (mat_left_row_size != mat_right.row_size() ||
      mat_left.column_size() != mat_right_column_size) {
    throw std::invalid_argument("Matrix sizes do not match");
  }
  return std::make_unique<LazyOperation>(mat_left, mat_right, sum_mat_mat,
                                         sum_mat_mat_row, mat_left_row_size,
                                         mat_right_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::sum(const ComplexVectMatrix &mat_left,
                   const LazyOperation &mat_right) {
  int mat_left_row_size = mat_left.row_size();
  int mat_right_column_size = mat_right.column_size();
  if (mat_left_row_size != mat_right.row_size() ||
      mat_left.column_size() != mat_right_column_size) {
    throw std::invalid_argument("Matrix sizes do not match");
  }
  auto lazy = std::make_unique<LazyOperation>(mat_left);
  lazy->append(mat_right, sum_op_op, sum_op_op_row, sum_row_size,
               sum_column_size);
  return std::move(lazy);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const ComplexVectMatrix &mat_left,
                              const ComplexVectMatrix &mat_right) {
  int final_row_size = mat_left.row_size() * mat_right.row_size();
  int final_column_size = mat_left.column_size() * mat_right.column_size();
  return std::make_unique<LazyOperation>(
      mat_left, mat_right, tensor_product_mat_mat, tensor_product_mat_mat_row,
      final_row_size, final_column_size);
}

std::unique_ptr<LazyOperation>
AlgebraEngine::tensor_product(const ComplexVectMatrix &mat, const int times) {
  if (times == 1) {
    return std::make_unique<LazyOperation>(mat);
  }

  int final_row_size = mat.row_size() * mat.row_size();
  int final_column_size = mat.column_size() * mat.column_size();

  auto lazy = std::make_unique<LazyOperation>(
      mat, mat, tensor_product_mat_mat, tensor_product_mat_mat_row,
      final_row_size, final_column_size);

  for (int _ = 2; _ < times; _++) {
    lazy->append(mat, tensor_product_op_mat, tensor_product_op_mat_row,
                 tensor_product_final_row_size,
                 tensor_product_final_column_size);
  }
  return lazy;
}

bool AlgebraEngine::is_unitary(const ComplexVectMatrix &mat) {
  if (mat.row_size() != mat.column_size()) {
    return false;
  }

  auto a_dagger = AlgebraEngine::conjugate_transpose(mat);
  auto lazy = std::make_unique<LazyOperation>(mat);
  lazy->append(*a_dagger, matrix_multiplication_op_op,
               matrix_multiplication_op_op_row,
               matrix_multiplication_final_row_size,
               matrix_multiplication_final_column_size);

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

    /*
    if (!approx_equal(lazy->get(m, m), Complex(1, 0))) {
      return false;
    }
    auto row_split = lazy->get(m, 0, m, lazy->column_size() - 1);
    __complex_precision sum_real, sum_imag = 0;
    vDSP_sve(row_split.real().data(), 1, &sum_real, row_split.size());
    vDSP_sve(row_split.imag().data(), 1, &sum_imag, row_split.size());

    if (!approx_equal(Complex(sum_real, sum_imag), Complex(1, 0))) {
      return false;
    }
    */
  }
  return true;
}
