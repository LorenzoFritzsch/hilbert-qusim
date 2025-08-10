#ifndef LAZY_OPERATION_H
#define LAZY_OPERATION_H

#include "complex_vector_split.h"
#include "complex_vectorised_matrix.h"
#include "operation.h"
#include <functional>
#include <memory>
#include <vector>

class LazyOperation final {
public:
  using op_op = std::function<Complex(
      const Operation &left, const Operation &right, const int m, const int n)>;
  using op_mat = std::function<Complex(const Operation &left,
                                       const ComplexVectMatrix &right,
                                       const int m, const int n)>;
  using mat_op =
      std::function<Complex(const ComplexVectMatrix &left,
                            const Operation &right, const int m, const int n)>;
  using mat_mat = std::function<Complex(const ComplexVectMatrix &left,
                                        const ComplexVectMatrix &right,
                                        const int m, const int n)>;

  using op_op_row = std::function<ComplexVectSplit(
      const Operation &left, const Operation &right, const int row)>;
  using op_mat_row = std::function<ComplexVectSplit(
      const Operation &left, const ComplexVectMatrix &right, const int row)>;
  using mat_op_row = std::function<ComplexVectSplit(
      const ComplexVectMatrix &left, const Operation &right, const int row)>;
  using mat_mat_row = std::function<ComplexVectSplit(
      const ComplexVectMatrix &left, const ComplexVectMatrix &right,
      const int row)>;

  // TODO: Review
  LazyOperation(LazyOperation &&) = default;
  LazyOperation &operator=(LazyOperation &&) = default;
  LazyOperation(const LazyOperation &) = delete;
  LazyOperation &operator=(const LazyOperation &) = delete;

  LazyOperation(const ComplexVectMatrix &left, const ComplexVectMatrix &right,
                mat_mat op, mat_mat_row op_row, const int final_row_size,
                const int final_column_size) {
    mat_vect_.push_back(left);
    mat_vect_.push_back(right);
    op_vect_.emplace_back(0, 1, mat_vect_, op_vect_, std::move(op),
                          std::move(op_row), final_row_size, final_column_size);
  }

  explicit LazyOperation(const ComplexVectMatrix &mat) {
    mat_vect_.push_back(mat);
    op_vect_.emplace_back(
        0, 0, mat_vect_, op_vect_,
        [this](const ComplexVectMatrix &left, const ComplexVectMatrix &,
               const int m, const int n) { return mat_vect_[0].get(m, n); },
        [this](const ComplexVectMatrix &left, const ComplexVectMatrix &right,
               const int row) { return mat_vect_[0].get_row(row); },
        mat.row_size(), mat.column_size());
  }

  LazyOperation(const ComplexVectMatrix &mat, mat_mat_row op_row) {
    mat_vect_.push_back(mat);
    op_vect_.emplace_back(
        0, 0, mat_vect_, op_vect_,
        [this](const ComplexVectMatrix &left, const ComplexVectMatrix &,
               const int m, const int n) { return mat_vect_[0].get(m, n); },
        op_row, mat.row_size(), mat.column_size());
  }

  void append(const ComplexVectMatrix &mat, op_mat op, op_mat_row op_row,
              std::function<int(int, int, int, int)> row_size,
              std::function<int(int, int, int, int)> column_size) {
    mat_vect_.push_back(mat);
    auto op_index = static_cast<int>(op_vect_.size()) - 1;
    auto mat_index = static_cast<int>(mat_vect_.size()) - 1;
    auto final_row_size = row_size(op_vect_[op_index].row_size(),
                                   op_vect_[op_index].column_size(),
                                   mat.row_size(), mat.column_size());
    auto final_column_size = column_size(op_vect_[op_index].row_size(),
                                         op_vect_[op_index].column_size(),
                                         mat.row_size(), mat.column_size());
    op_vect_.emplace_back(op_index, mat_index, mat_vect_, op_vect_,
                          std::move(op), std::move(op_row), final_row_size,
                          final_column_size);
  }

  void append(const LazyOperation &lazy_op, op_op op, op_op_row op_row,
              std::function<int(int, int, int, int)> row_size,
              std::function<int(int, int, int, int)> column_size);

  [[nodiscard]] Complex get(const int m, const int n) const {
    return op_vect_.back().get(m, n);
  }

  [[nodiscard]] ComplexVectSplit get(const int row) const {
    return op_vect_.back().get(row);
  }

  [[nodiscard]] int row_size() const { return op_vect_.back().row_size(); }

  [[nodiscard]] int column_size() const {
    return op_vect_.back().column_size();
  }

  [[nodiscard]] std::unique_ptr<ComplexVectMatrix> to_matrix() const;

  [[nodiscard]] std::vector<ComplexVectMatrix> mat_vect() { return mat_vect_; }

  [[nodiscard]] std::vector<Operation> op_vect() const { return op_vect_; }

private:
  std::vector<ComplexVectMatrix> mat_vect_;
  std::vector<Operation> op_vect_;
};
#endif // !LAZY_OPERATION_H
