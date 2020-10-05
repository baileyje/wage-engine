#pragma once

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"

#define TransformComponent 0

namespace wage {
  namespace math {

    class Transform {

    public:
      Vector3 localPosition;

      Vector3 localScale;

      Quaternion localRotation;

      Transform(Vector3 position, Vector3 scale, Vector3 eulerRotation);

      Transform(Vector3 position, Vector3 scale, Quaternion rotation);

      Transform();

      ~Transform();

      Vector position(Vector3 parentPosition = Vector3::Zero) const;

      Vector3 scale(Vector3 parentScale = Vector3::One) const;

      Quaternion rotation(Quaternion parentQuat = Quaternion()) const;

      void setEulerRotation(Vector3 eulers);

      Matrix localProjection() const;

      Matrix worldProjection(Transform parent = Transform{}) const;
    };

  }
}
