#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// TODO: Move to engine math
#include <glm/glm.hpp>

namespace wage {

  namespace render {

    struct UniformBufferObject {
      alignas(16) glm::mat4 model;
      alignas(16) glm::mat4 view;
      alignas(16) glm::mat4 proj;
    };
  }
}