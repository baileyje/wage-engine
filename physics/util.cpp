#include "physics/util.h"


btTransform fromTransform(Transform* transform) {
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(fromVector(*transform->getPosition()));
  btQuaternion rotation(
    btRadians(transform->rotation.y), 
    btRadians(transform->rotation.x), 
    btRadians(transform->rotation.z)
  );
  btTransform.setRotation(rotation);
  return btTransform;
}