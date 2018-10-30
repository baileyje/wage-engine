#include "physics/util.h"


btTransform fromTransform(Transform* transform) {
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(fromVector(transform->getWorldPosition()));  
  Quaternion transformRotation = transform->getWorldRotation();
  btQuaternion rotation(
    transformRotation.x,
    transformRotation.y,
    transformRotation.z,
    transformRotation.w
  );
  btTransform.setRotation(rotation);  
  return btTransform;
}