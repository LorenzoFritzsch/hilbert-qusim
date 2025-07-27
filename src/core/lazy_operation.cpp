#include "lazy_operation.h"
#include "complex_vectorised_matrix.h"

#include <memory>
#include <thread>

void LazyOperation::append(const LazyOperation &lazy_op, op_op op,
                           op_op_row op_row,
                           std::function<int(int, int, int, int)> row_size,
                           std::function<int(int, int, int, int)> column_size) {

  // Add all elements of lazy_op and operations
  auto sub_op_start_index = static_cast<int>(op_vect_.size());
  auto lazy_op_operations = lazy_op.op_vect();
  for (int i = 0; i < lazy_op_operations.size(); i++) {
    auto operation = lazy_op_operations[i];
    auto op_next = sub_op_start_index + i;
    auto mat_next = static_cast<int>(mat_vect_.size());
    switch (operation.op_type()) {
    case OperationMatrix: {
      mat_vect_.push_back(std::get<ComplexVectMatrix>(operation.right()));
      op_vect_.emplace_back(op_next - 1, mat_next, mat_vect_, op_vect_,
                            std::get<op_mat>(operation.op_functor()),
                            std::get<op_mat_row>(operation.op_row_functor()),
                            operation.row_size(), operation.column_size());
      break;
    }
    case MatrixMatrix: {
      mat_vect_.push_back(std::get<ComplexVectMatrix>(operation.left()));
      mat_vect_.push_back(std::get<ComplexVectMatrix>(operation.right()));
      op_vect_.emplace_back(mat_next, mat_next + 1, mat_vect_, op_vect_,
                            std::get<mat_mat>(operation.op_functor()),
                            std::get<mat_mat_row>(operation.op_row_functor()),
                            operation.row_size(), operation.column_size());
      break;
    }
    case OperationOperation: {
      auto left_index = sub_op_start_index + operation.left_index();
      auto right_index = sub_op_start_index + operation.right_index();
      op_vect_.emplace_back(left_index, right_index, mat_vect_, op_vect_,
                            std::get<op_op>(operation.op_functor()),
                            std::get<op_op_row>(operation.op_row_functor()),
                            operation.row_size(), operation.column_size());
    }
    case MatrixOperation:
      // Unused
      break;
    }
  }

  // Now, the last op represents the input lazy operation.
  auto sub_op_end_index =
      sub_op_start_index + static_cast<int>(lazy_op_operations.size()) - 1;
  auto op_row_size = op_vect_.back().row_size();
  auto op_column_size = op_vect_.back().column_size();
  auto lazy_row_size = lazy_op.row_size();
  auto lazy_column_size = lazy_op.column_size();
  auto final_row_size =
      row_size(op_row_size, op_column_size, lazy_row_size, lazy_column_size);
  auto final_column_size =
      column_size(op_row_size, op_column_size, lazy_row_size, lazy_column_size);
  op_vect_.emplace_back(sub_op_start_index - 1, sub_op_end_index, mat_vect_,
                        op_vect_, op, op_row, final_row_size,
                        final_column_size);
}

std::unique_ptr<ComplexVectMatrix> LazyOperation::to_matrix() const {
  const auto elements_size = row_size() * column_size();
  const auto num_threads =
      static_cast<int>(std::thread::hardware_concurrency());
  const auto chunk_size = static_cast<int>(
      std::ceil(static_cast<double>(elements_size) / num_threads));
  auto threads = std::vector<std::thread>(num_threads);

  auto result = ComplexVector(elements_size);
  for (int i = 0; i < row_size(); i++) {
    auto res = get(i).get();
    result.insert(result.begin() + i * column_size(), res.begin(), res.end());
  }
  /*
  for (int t = 0; t < num_threads; t++) {
    int start_index = t * chunk_size;
    int end_index = std::min(start_index + chunk_size, elements_size);
    if (start_index >= elements_size) {
      break;
    }

    //    threads[t] = std::thread([&result, start_index, end_index, this] {
    auto res = get(start_index / column_size(), start_index % column_size(),
                   end_index / column_size(), end_index % column_size())
                   ->get();
    result.insert(result.begin() + start_index, res.begin(), res.end());
    //   });
  }
  */

  /*
  for (auto &t : threads) {
    if (t.joinable())
      t.join();
  }
  */

  return std::make_unique<ComplexVectMatrix>(result, row_size(), column_size());
}
