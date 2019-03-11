#ifndef MATH_PLANE_H
#define MATH_PLANE_H

#include <iostream>

#include "math/vector.h"

namespace wage {

  class Plane {

  public:
    Plane() {}

    float distance(const Vector& point) const {
      return Vector::dot(point, normal) + d;
    }

    Vector normal;

    float d;

  };
}

#endif //MATH_PLANE_H
