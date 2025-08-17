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

#ifndef HILBERT_NAMESPACE_TEST_H
#define HILBERT_NAMESPACE_TEST_H

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "lazy_operation.h"
#include <cstddef>
#include <iostream>

#include <chrono>

/*
 * Formats a delta from microseconds to seconds.
 */
inline std::string mussec(unsigned long long delta_mus) {
  double delta_sec = static_cast<double>(delta_mus) / 1000000;
  return std::to_string(delta_sec);
}

class PerfTest {
public:
  explicit PerfTest(const std::string title)
      : title_(title), start_(std::chrono::high_resolution_clock::now()) {}

  ~PerfTest() {
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
    const auto duration_seconds = mussec(duration.count());
    std::cout << std::endl
              << "** Elapsed time " << title_ << ": \033[1m" << duration_seconds
              << " seconds\033[0m";
  }

private:
  const std::string title_;
  const std::chrono::high_resolution_clock::time_point start_;
};

inline void print_info(const std::string info) {
#ifdef PERFORMANCE_TESTING
  std::cout << std::endl << "-> " << info;
#endif
}

inline PerfTest *pt_start(const std::string title) {
#ifdef PERFORMANCE_TESTING
  return new PerfTest(title);
#else
  return nullptr;
#endif
}

inline void pt_stop(PerfTest *pt) {
  if (pt != nullptr) {
    delete pt;
  }
}

inline bool are_matrices_equal(const ComplexVectMatrix &left,
                               const LazyOperation &right) {
  if (left.row_size() != right.row_size() ||
      left.column_size() != right.column_size()) {
    return false;
  }
  for (size_t i = 0; i < left.row_size(); i++) {
    for (size_t j = 0; j < left.column_size(); j++) {
      if (left.get(i, j) != right.get(i, j)) {
        return false;
      }
    }
  }
  return true;
}

inline bool verify_identity_matrix(const ComplexVectMatrix &matrix,
                                   size_t expected_size) {
  if (matrix.row_size() != matrix.column_size() ||
      matrix.row_size() != expected_size) {
    return false;
  }
  for (auto m = 0; m < matrix.row_size(); m++) {
    for (auto n = 0; n < matrix.column_size(); n++) {
      if (m == n) {
        if (matrix.get(m, n) != Complex(1, 0)) {
          return false;
        }
      } else {
        if (matrix.get(m, n) != Complex(0, 0)) {
          return false;
        }
      }
    }
  }
  return true;
}

inline void run_test(const std::string &title, std::function<bool()> test,
                     int &failed, int &total) {
  std::cout << title << std::string(60 - title.length(), '.');
  try {
    auto result = test();
#ifdef PERFORMANCE_TESTING
    std::cout << std::endl << std::string(60, '.');
#endif
    if (result) {
      std::cout << "\033[32m PASSED\033[0m";
    } else {
      failed += 1;
      std::cout << "\033[31m FAILED\033[0m";
    }
    std::cout << std::endl;
  } catch (const std::exception &e) {
    std::cout << "\033[31m FAILED: " << e.what() << "\033[0m" << std::endl;
  }
  total += 1;
}

inline void run_test(const std::string &title, std::function<bool()> test,
                     int &failed, int &total,
                     const bool skip_when_perf_testing) {
#ifdef PERFORMANCE_TESTING
  if (skip_when_perf_testing) {
    return;
  }
#endif
  run_test(title, test, failed, total);
}

inline void test_resumen(const int &failed, const int &total) {
  std::cout << "\033[1mRun: " << total << ", failed: " << failed << "\033[0m"
            << std::endl;
}

#endif // !HILBERT_NAMESPACE_TEST_H
