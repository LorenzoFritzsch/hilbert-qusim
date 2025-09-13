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

#include "complex_vector_split.h"
#include "hilbert_namespace_test.h"

bool it_should_create_cvs_from_complex_vector() {
  // Given
  std::vector<Complex> vect = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

  // When
  auto cvs = ComplexVectSplit(vect);

  // Then
  std::vector<__complex_precision> real_exp = {0, 0, 1, 1};
  std::vector<__complex_precision> imag_exp = {0, 1, 0, 1};
  auto real_act = cvs.real();
  auto imag_act = cvs.imag();

  return real_exp == real_act && imag_exp == imag_act;
}

bool it_should_create_cvs_from_split_vectors() {
  // Given
  std::vector<__complex_precision> real = {0, 0, 1, 1};
  std::vector<__complex_precision> imag = {0, 1, 0, 1};

  // When
  auto cvs = ComplexVectSplit(real, imag);

  // Then
  auto real_act = cvs.real();
  auto imag_act = cvs.imag();
  return real == real_act && imag == imag_act;
}

bool it_should_conjugate_cvs() {
  // Given
  std::vector<Complex> vect = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

  // When
  auto cvs = ComplexVectSplit(vect);
  auto cvs_conj = cvs.conj();

  // Then
  std::vector<Complex> conj_exp = {{0, 0}, {0, -1}, {1, 0}, {1, -1}};
  std::vector<Complex> conj_act = *cvs_conj->get();
  return conj_exp == conj_act;
}

int main() {
  int total = 0;
  int failed = 0;

  run_test("it_should_create_cvs_from_complex_vector",
           it_should_create_cvs_from_complex_vector, failed, total);

  run_test("it_should_create_cvs_from_split_vectors",
           it_should_create_cvs_from_split_vectors, failed, total);

  run_test("it_should_conjugate_cvs", it_should_conjugate_cvs, failed, total);

  test_resumen(failed, total);
  return failed == 0 ? 0 : 1;
}
