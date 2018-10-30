#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

typedef glm::quat Quaternion;

static Quaternion quatFromEulers(Vector vector) {    
  return Quaternion(Vector(
    glm::radians(vector.x),
    glm::radians(vector.y),
    glm::radians(vector.z)
  ));
}

#endif //MATH_QUATERNION_H