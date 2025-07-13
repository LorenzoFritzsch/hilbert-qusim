#include "lazy_operation.h"
#include "complex_vectorised_matrix.h"

#include <memory>
#include <thread>

std::unique_ptr<ComplexVectMatrix> LazyOperation::to_matrix() const {
  const auto elements_size = row_size() * column_size();
  const auto num_threads =
      static_cast<int>(std::thread::hardware_concurrency());
  const auto chunk_size = static_cast<int>(
      std::ceil(static_cast<double>(elements_size) / num_threads));
  auto threads = std::vector<std::thread>(num_threads);

  auto result = ComplexVector(elements_size);
  for (int t = 0; t < num_threads; t++) {
    int start_index = t * chunk_size;
    int end_index = std::min(start_index + chunk_size, elements_size);
    if (start_index >= elements_size) {
      break;
    }

    threads[t] = std::thread([&result, start_index, end_index, this] {
      for (int i = start_index; i < end_index; ++i) {
        result[i] = get(i / column_size(), i % column_size());
      }
    });
  }

  for (auto &t : threads) {
    if (t.joinable())
      t.join();
  }

  return std::make_unique<ComplexVectMatrix>(result, row_size(), column_size());
}
