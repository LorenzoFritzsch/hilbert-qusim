#ifndef OPERATION_H
#define OPERATION_H

#include "operation_member.h"
#include <functional>
#include <memory>

class Operation final : public OpMember {
public:
  Operation(
      std::unique_ptr<OpMember> left, std::unique_ptr<OpMember> right,
      std::function<Complex(const std::unique_ptr<OpMember> &a,
                            const std::unique_ptr<OpMember> &b, int m, int n)>
          op,
      const int final_row_size, const int final_column_size)
      : left_(std::move(left)), right_(std::move(right)),
        row_size_(final_row_size), column_size_(final_column_size),
        op_(std::move(op)) {}

  Operation(Operation &&) = default;
  Operation &operator=(Operation &&) = default;

  Operation(const Operation &) = delete;
  Operation &operator=(const Operation &) = delete;

  [[nodiscard]] Complex get(const int m, const int n) const override {
    return op_(left_, right_, m, n);
  }

  [[nodiscard]] int row_size() const override { return row_size_; }

  [[nodiscard]] int column_size() const override { return column_size_; }

  [[nodiscard]] std::unique_ptr<OpMember> clone() const override {
    return std::make_unique<Operation>(left_->clone(), right_->clone(), op_,
                                       row_size_, column_size_);
  }

  /*
   * Transfers ownership of `left_`.
   */
  [[nodiscard]] std::unique_ptr<OpMember> get_left() {
    return std::move(left_);
  }

  /*
   * Transfers ownership of `right_`.
   */
  [[nodiscard]] std::unique_ptr<OpMember> get_right() {
    return std::move(right_);
  }

  [[nodiscard]] std::function<Complex(const std::unique_ptr<OpMember> &a,
                                      const std::unique_ptr<OpMember> &b, int m,
                                      int n)>
  get_op() {
    return op_;
  }

private:
  std::unique_ptr<OpMember> left_;
  std::unique_ptr<OpMember> right_;
  int row_size_;
  int column_size_;
  std::function<Complex(const std::unique_ptr<OpMember> &a,
                        const std::unique_ptr<OpMember> &b, int m, int n)>
      op_;
};

#endif // !OPERATION_H
