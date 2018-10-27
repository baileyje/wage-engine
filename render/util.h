#ifndef RENDER_UTIL_H
#define RENDER_UTIL_H

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "math/vector.h"
#include "entity/color.h"

void checkGLError(const char* command);


#define FAIL_CHECK(cmd)    \
    do                     \
    {                      \
        cmd;               \
        checkGLError(#cmd); \
    } while(0)              \


inline glm::vec3 vec3From(Vector* vector) {
    return glm::vec3(vector->x, vector->y, vector->z);
}

inline glm::vec3 vec3From(Color* color) {
    return glm::vec3(color->r, color->g, color->b);
}

inline glm::vec3 directionFromEulers(Vector* vector) {
  float yaw = glm::radians(vector->x);
  float pitch = glm::radians(vector->y);
    return glm::vec3(
        glm::cos(yaw) * glm::cos(pitch),
        glm::sin(yaw) * glm::cos(pitch),
        glm::sin(pitch)
    );
}

inline glm::quat quatFromEulers(Vector* vector) {
  return glm::quat(glm::vec3(
    glm::radians(vector->x),
    glm::radians(vector->y),
    glm::radians(vector->z)
  ));
}

#endif //RENDER_UTIL_H
