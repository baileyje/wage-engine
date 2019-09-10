#pragma once

#include "math/vector.h"

namespace wage { namespace math {

  class BoundingBox {

  public:
    BoundingBox(Vector position, Vector halfDim) : position(position), halfDim(halfDim) {
    }

    Vector position;

    Vector halfDim;
  };

} }