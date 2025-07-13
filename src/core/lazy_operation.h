#ifndef LAZY_OPERATION_H
#define LAZY_OPERATION_H

#include "complex_vectorised_matrix.h"
#include "operation.h"
#include "operation_member.h"
#include <memory>

class LazyOperation final : public OpMember {
public:
  LazyOperation(std::unique_ptr<OpMember> left, std::unique_ptr<OpMember> right,
                const std::function<Complex(const std::unique_ptr<OpMember> &a,
                                            const std::unique_ptr<OpMember> &b,
                                            int m, int n)> &op,
                const int final_row_size, const int final_column_size)
      : operation_(
            std::make_unique<Operation>(std::move(left), std::move(right), op,
                                        final_row_size, final_column_size)) {}

  explicit LazyOperation(std::unique_ptr<OpMember> op) {
    int final_row_size = op->row_size();
    int final_column_size = op->column_size();
    operation_ = std::make_unique<Operation>(
        std::move(op), nullptr,
        [](const std::unique_ptr<OpMember> &a,
           const std::unique_ptr<OpMember> &, int m,
           int n) { return a->get(m, n); },
        final_row_size, final_column_size);
  }

  LazyOperation(LazyOperation &&) = default;
  LazyOperation &operator=(LazyOperation &&) = default;

  LazyOperation(const LazyOperation &) = delete;
  LazyOperation &operator=(const LazyOperation &) = delete;

  void
  push(std::unique_ptr<OpMember> operand,
       const std::function<Complex(const std::unique_ptr<OpMember> &a,
                                   const std::unique_ptr<OpMember> &b, int m,
                                   int n)> &op,
       const std::function<int(OpMember &a, OpMember &b)> &final_row_size,
       const std::function<int(OpMember &a, OpMember &b)> &final_column_size) {
    int row_size = final_row_size(*operation_, *operand);
    int column_size = final_column_size(*operation_, *operand);
    operation_ = std::make_unique<Operation>(
        std::move(operation_), std::move(operand), op, row_size, column_size);
  }

  [[nodiscard]] Complex get(const int m, const int n) const override {
    return operation_->get(m, n);
  }

  [[nodiscard]] int row_size() const override { return operation_->row_size(); }

  [[nodiscard]] int column_size() const override {
    return operation_->column_size();
  }

  [[nodiscard]] std::unique_ptr<OpMember> clone() const override {
    std::unique_ptr<Operation> op(
        dynamic_cast<Operation *>(operation_->clone().release()));
    return std::make_unique<LazyOperation>(op->get_left(), op->get_right(),
                                           op->get_op(), op->row_size(),
                                           op->column_size());
  }

  [[nodiscard]] std::unique_ptr<ComplexVectMatrix> to_matrix() const;

private:
  std::unique_ptr<Operation> operation_;
};
#endif // !LAZY_OPERATION_H
