#pragma once
#include "math/vector.h"

namespace wage { namespace math {

  class BoundingSphere {

  public:
    BoundingSphere(Vector position, float radius) : position(position), radius(radius) {
    }

    Vector position;

    float radius;
  };

} }
