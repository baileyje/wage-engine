#pragma once

#include <btBulletDynamicsCommon.h>

#include "physics/rigid_body.h"
#include "math/transform.h"

namespace wage {
  namespace physics {

    inline btVector3 fromVector(math::Vector vector) {
      return btVector3(vector.x, vector.y, vector.z);
    }

    inline math::Vector fromBTVector(btVector3 vector) {
      return math::Vector(vector.x(), vector.y(), vector.z());
    }

    btRigidBody* createRigidBody(RigidBody* rigidBody, const btTransform& startTransform, btCollisionShape* shape);

    btTransform fromTransform(math::Transform* transform);

  }
}
