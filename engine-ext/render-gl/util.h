#pragma once

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>

#include "math/vector.h"
#include "math/quaternion.h"
#include "component/color.h"

namespace wage {
  namespace render {

    void checkGLError(const std::string& filename, const std::uint_fast32_t line, const char* command);

#define GL_FAIL_CHECK(cmd)                  \
  do {                                      \
    cmd;                                    \
    checkGLError(__FILE__, __LINE__, #cmd); \
  } while (0)

    inline math::Vector vec3From(const component::Color& color) {
      return math::Vector(color.r, color.g, color.b);
    }

    inline math::Vector directionFromEulers(math::Vector vector) {
      float yaw = vector.x;
      float pitch = vector.y;
      return math::Vector(
          glm::cos(yaw) * glm::cos(pitch),
          glm::sin(yaw) * glm::cos(pitch),
          glm::sin(pitch));
    }

  }
}
