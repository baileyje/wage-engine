#pragma once

#include <vector>

#include "math/transform.h"

namespace wage::math {

  class TransformChain {
  public:
    TransformChain(std::vector<Transform> chain);

    Matrix worldProjection() const;

  private:
    const std::vector<Transform> chain;
  };
}