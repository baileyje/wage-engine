#pragma once

#include <btBulletDynamicsCommon.h>

#include "physics/components/rigid_body.h"
#include "math/transform.h"

namespace wage {
  namespace physics {

    inline btVector3 fromVector(math::Vector vector) {
      return btVector3(vector.x, vector.y, vector.z);
    }

    inline math::Vector fromBTVector(btVector3 vector) {
      return math::Vector(vector.x(), vector.y(), vector.z());
    }

    inline btTransform fromTransform(math::Transform* transform) {
      btTransform btTransform;
      btTransform.setIdentity();
      btTransform.setOrigin(fromVector(transform->position()));
      math::Quaternion transformRotation = transform->rotation();
      btQuaternion rotation(transformRotation.x, transformRotation.y, transformRotation.z, transformRotation.w);
      btTransform.setRotation(rotation);
      return btTransform;
    }

  }
}
