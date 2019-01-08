#ifndef MATH_BOUNDING_BOX_H
#define MATH_BOUNDING_BOX_H

#include "math/vector.h"

namespace wage {

  class BoundingBox {

  public:

    BoundingBox(Vector position, Vector halfDim) : position(position), halfDim(halfDim) {      
    }

    Vector position;

    Vector halfDim;

  };

}

#endif //MATH_BOUNDING_BOX_H