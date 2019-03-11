#include "physics-bullet/util.h"

namespace wage {

  btTransform fromTransform(Transform& transform) {
    btTransform btTransform;
    btTransform.setIdentity();
    btTransform.setOrigin(fromVector(transform.position()));
    Quaternion transformRotation = transform.rotation();
    btQuaternion rotation(transformRotation.x, transformRotation.y, transformRotation.z, transformRotation.w);
    btTransform.setRotation(rotation);
    return btTransform;
  }
}