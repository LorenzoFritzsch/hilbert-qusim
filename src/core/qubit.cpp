#include "qubit.h"

std::unique_ptr<ComplexVector> Qubit::to_vector() const {
  return std::make_unique<ComplexVector>(std::initializer_list{alpha, beta});
}
