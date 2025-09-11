// Copyright 2025 Lorenzo Fritzsch
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "lazy_operation.h"
#include "complex_vectorised_matrix.h"

#include <memory>
#include <thread>

void LazyOperation::append(const LazyOperation &lazy_op, op_op op,
                           op_op_row op_row, const size_t final_row_size,
                           const size_t final_column_size) {

  // Add all elements of lazy_op and operations
  auto sub_op_start_index = op_vect_.size();
  auto lazy_op_operations = lazy_op.op_vect();
  for (size_t i = 0; i < lazy_op_operations.size(); i++) {
    auto operation = lazy_op_operations[i];
    auto op_next = sub_op_start_index + i;
    auto mat_next = mat_vect_.size();
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
      throw std::logic_error("MatrixOperation append not implemented.");
    }
  }

  // Now, the last op represents the input lazy operation.
  auto sub_op_end_index = sub_op_start_index + lazy_op_operations.size() - 1;
  op_vect_.emplace_back(sub_op_start_index - 1, sub_op_end_index, mat_vect_,
                        op_vect_, op, op_row, final_row_size,
                        final_column_size);
}

std::unique_ptr<ComplexVectMatrix> LazyOperation::to_matrix() const {
  const auto num_threads = std::thread::hardware_concurrency();
  const auto rows_per_thread =
      static_cast<int>(std::ceil(static_cast<float>(row_size()) / num_threads));
  auto threads = std::vector<std::thread>(num_threads);

  auto result = ComplexVector(row_size() * column_size());
  auto total_rows = row_size();
  for (size_t t = 0; t < num_threads; t++) {
    size_t start_row = t * rows_per_thread;
    size_t end_row = std::min(start_row + rows_per_thread, total_rows);

    threads[t] = std::thread([&result, start_row, end_row, total_rows, this] {
      for (size_t n = start_row; n < end_row; n++) {
        auto res = get(n)->get();
        std::copy(res->begin(), res->end(), result.begin() + (n * total_rows));
      }
    });
  }
  for (auto &t : threads) {
    if (t.joinable())
      t.join();
  }

  return std::make_unique<ComplexVectMatrix>(result, row_size(), column_size());
}
