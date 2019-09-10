#pragma once

#include <iostream>

#include "math/vector.h"

namespace wage { namespace math {

  class Plane {

  public:
    Plane() {}

    float distance(const Vector& point) const {
      return Vector::dot(point, normal) + d;
    }

    Vector normal;

    float d;

  };

} }