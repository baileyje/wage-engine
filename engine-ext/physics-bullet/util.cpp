#include "physics-bullet/util.h"

namespace wage { namespace physics {

  btTransform fromTransform(math::Transform* transform) {
    btTransform btTransform;
    btTransform.setIdentity();
    btTransform.setOrigin(fromVector(transform->position()));
    math::Quaternion transformRotation = transform->rotation();
    btQuaternion rotation(transformRotation.x, transformRotation.y, transformRotation.z, transformRotation.w);
    btTransform.setRotation(rotation);
    return btTransform;
  }

} }