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
#include "state_vector.h"
#include <cstddef>
#include <iostream>

#include <chrono>
#include <ostream>
#include <string>

/*
 * Formats a delta from microseconds to seconds.
 */
inline std::string mussec(unsigned long long delta_mus) {
  double delta_sec = static_cast<double>(delta_mus) / 1000000;
  return std::to_string(delta_sec);
}

/*
 * Prints out a state vector.
 */
inline void svout(const StateVector &v, const std::string &title) {
  const auto size = v.size();
  const auto qbits = v.get();
  std::cout << std::endl << "\033[1m" << title << "\033[0m" << std::endl;
  auto sq2 = 1 / std::sqrt(2);
  for (int i = size - 1; i >= 0; i--) {
    auto q = *v[i].to_vector();
    auto alpha = q.get(0, 0);
    auto beta = q.get(0, 1);
    alpha = approx_equal(alpha, 0) ? 0 : alpha;
    alpha = approx_equal(alpha, 1) ? 1 : alpha;
    beta = approx_equal(beta, 0) ? 0 : beta;
    beta = approx_equal(beta, 1) ? 1 : beta;
    auto na = std::norm(alpha);
    auto nb = std::norm(beta);
    std::string ket0 = "\033[1;90m|0>\033[0m";
    std::string ket1 = "\033[1;90m|1>\033[0m";
    if (na > nb) {
      ket0 = "\033[1;97m|0>\033[0m";
    } else if (na < nb) {
      ket1 = "\033[1;97m|1>\033[0m";
    }
    std::string r = "";
    if (approx_equal(nb, 0)) {
      r = " => \033[96m|0>\033[0m";
    } else if (approx_equal(na, 0)) {
      r = " => \033[96m|1>\033[0m";
    } else if (approx_equal(na, nb)) {
      if (approx_equal(alpha, sq2) && approx_equal(beta, sq2)) {
        r = " => \033[96m|+>\033[0m";
      } else if (approx_equal(alpha, sq2) && approx_equal(beta, -sq2)) {
        r = " => \033[96m|->\033[0m";
      }
    }

    std::cout << std::to_string(i) << "---------\033[94m alpha: \033[0m"
              << (alpha.real() >= 0 ? " " : "") << std::to_string(alpha.real())
              << (alpha.imag() >= 0 ? " + " : " - ")
              << std::to_string(std::abs(alpha.imag())) << "i"
              << ",\033[94m beta: \033[0m" << (beta.real() >= 0 ? " " : "")
              << std::to_string(beta.real())
              << (beta.imag() >= 0 ? " + " : " - ")
              << std::to_string(std::abs(beta.imag())) << "i ";

    std::cout << "\033[95m squared norms: \033[0m"
              << std::to_string(std::norm(alpha)) << ket0 << ", "
              << std::to_string(std::norm(beta)) << ket1 << r << std::endl;
  }
}

/*
 * Prints out a string
 */
inline void print(const std::string &msg) {
  std::cout << std::endl << msg << std::endl;
}

/*
 * Prints out a matrix
 */
inline void mxout(const ComplexVectMatrix &mat, const std::string &title) {
  std::cout << std::endl << "\033[1m" << title << "\033[0m" << std::endl;
  for (size_t m = 0; m < mat.row_size(); m++) {
    for (size_t n = 0; n < mat.column_size(); n++) {
      std::cout << "(" << std::to_string(mat.get(m, n).real()) << ", "
                << std::to_string(mat.get(m, n).imag()) << ") ";
    }
    std::cout << std::endl;
  }
}

/*
 * Prints out a lazy opration
 */
inline void loout(const LazyOperation &mat, const std::string &title) {
  std::cout << std::endl << "\033[1m" << title << "\033[0m" << std::endl;
  for (size_t m = 0; m < mat.row_size(); m++) {
    for (size_t n = 0; n < mat.column_size(); n++) {
      std::cout << "(" << std::to_string(mat.get(m, n).real()) << ", "
                << std::to_string(mat.get(m, n).imag()) << ") ";
    }
    std::cout << std::endl;
  }
}

/*
 * Prints out a lazy matrix casting all reals to int
 */
inline void loout_int(const LazyOperation &mat, const std::string &title) {
  std::cout << std::endl << "\033[1m" << title << "\033[0m" << std::endl;
  for (size_t m = 0; m < mat.row_size(); m++) {
    for (size_t n = 0; n < mat.column_size(); n++) {
      std::cout << std::to_string((int)mat.get(m, n).real()) << "  ";
    }
    std::cout << std::endl;
  }
}

/*
 * Prints out a lazy matrix's reals
 */
inline void loout_reals(const LazyOperation &mat, const std::string &title) {
  std::cout << std::endl << "\033[1m" << title << "\033[0m" << std::endl;
  for (size_t m = 0; m < mat.row_size(); m++) {
    for (size_t n = 0; n < mat.column_size(); n++) {
      std::cout << std::to_string(mat.get(m, n).real()) << "  ";
    }
    std::cout << std::endl;
  }
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
  return left == *right.to_matrix();
}

inline bool verify_identity_matrix(const ComplexVectMatrix &matrix,
                                   size_t expected_size) {
  if (matrix.row_size() != matrix.column_size() ||
      matrix.row_size() != expected_size) {
    return false;
  }

  for (size_t m = 0; m < matrix.row_size(); m++) {
    if (!approx_equal(matrix.get(m, m), Complex(1, 0))) {
      return false;
    }
    if (!approx_equal(simd::cvsve(*matrix.get_row(m)), Complex(1, 0))) {
      return false;
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
    failed += 1;
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
