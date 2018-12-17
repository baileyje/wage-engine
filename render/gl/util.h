#ifndef RENDER_UTIL_H
#define RENDER_UTIL_H

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>

#include "math/vector.h"
#include "math/quaternion.h"
#include "math/color.h"

namespace wage {

    void checkGLError(const char* command);

    #define GL_FAIL_CHECK(cmd)    \
        do                     \
        {                      \
            cmd;               \
            checkGLError(#cmd); \
        } while(0)              \


    inline Vector vec3From(Color* color) {
        return Vector(color->r, color->g, color->b);
    }

    inline Vector directionFromEulers(Vector vector) {
      float yaw = vector.x;
      float pitch = vector.y;
      return Vector(
          glm::cos(yaw) * glm::cos(pitch),
          glm::sin(yaw) * glm::cos(pitch),
          glm::sin(pitch)
      );
    }

}

#endif //RENDER_UTIL_H
