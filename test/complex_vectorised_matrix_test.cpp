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

#include "complex_vectorised_matrix.h"
#include "hilbert_namespace.h"
#include "hilbert_namespace_test.h"
#include <memory>

bool it_should_create_vectorised_matrix() {
  // When
  const auto m_vectorised = std::make_unique<ComplexVectMatrix>(identity_2x2);

  // Then
  return verify_identity_matrix(*m_vectorised, 2);
}

int main() {
  int total = 0;
  int failed = 0;

  run_test("it_should_create_vectorised_matrix",
           it_should_create_vectorised_matrix, failed, total);

  test_resumen(failed, total);
  return failed == 0 ? 0 : 1;
}
