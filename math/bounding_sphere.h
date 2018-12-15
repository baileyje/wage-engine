#ifndef MATH_BOUNDING_SPHERE_H
#define MATH_BOUNDING_SPHERE_H

#include "math/vector.h"

namespace wage {

  class BoundingSphere {

  public:

    BoundingSphere(Vector position, float radius) : position(position), radius(radius) {      
    }

    Vector position;

    float radius;

  };

}

#endif //MATH_BOUNDING_SPHERE_H