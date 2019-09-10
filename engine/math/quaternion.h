#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "math/vector.h"

namespace wage { namespace math {

  class Quaternion {

  public:
    float x;

    float y;

    float z;

    float w;

    Quaternion() : x(0), y(0), z(0), w(0) {}

    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Quaternion(Vector vector);

    static Quaternion fromEulers(const Vector& vector);

    Vector eulerAngles() const;

    Quaternion& operator*=(const Quaternion& rhs);

    friend Quaternion operator*(Quaternion lhs, const Quaternion& rhs) {
      lhs *= rhs;
      return lhs;
    }

    // TODO: Get rid of this someday
    inline glm::quat glm() const {
      return glm::quat(w, x, y, z);
    }

    Quaternion rotated(float angle, Vector axis) const;
  };

  Vector operator*(Quaternion quat, const Vector& vector);

} }
