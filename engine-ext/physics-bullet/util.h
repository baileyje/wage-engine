#ifndef PHYSICS_UTIL_H
#define PHYSICS_UTIL_H

#include <btBulletDynamicsCommon.h>

#include "physics/rigid_body.h"
#include "math/transform.h"

namespace wage {

  inline btVector3 fromVector(Vector vector) {
    return btVector3(vector.x, vector.y, vector.z);
  }

  inline Vector fromBTVector(btVector3 vector) {
    return Vector(vector.x(), vector.y(), vector.z());
  }

  btRigidBody* createRigidBody(RigidBody* rigidBody, const btTransform& startTransform, btCollisionShape* shape);

  btTransform fromTransform(Transform* transform);
}

#endif //PHYSICS_UTIL_H