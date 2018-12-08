#include "physics/util.h"

namespace wage {

  btTransform fromTransform(Transform& transform) {
    btTransform btTransform;
    btTransform.setIdentity();
    btTransform.setOrigin(fromVector(transform.getPosition()));  
    Quaternion transformRotation = transform.getRotation();
    btQuaternion rotation(
      transformRotation.x,
      transformRotation.y,
      transformRotation.z,
      transformRotation.w
    );
    btTransform.setRotation(rotation);    
    return btTransform;
  }

}