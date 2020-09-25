#pragma once

#include <array>
#include <unordered_map>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/mesh/vertex.h"

// TODO: Move to engine math
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace wage {
  namespace render {

    struct VulkanVertex : public Vertex {
    
      static VkVertexInputBindingDescription getBindingDescription();

      static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();

      // bool operator==(const GLVertex& other) const {
      //   return pos == other.pos && color == other.color && texCoord == other.texCoord;
      // }
    };
  }
}

