#pragma once

#include <array>
#include <unordered_map>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render/mesh/vertex.h"

namespace wage::render::vulkan {

  struct VulkanVertex : public Vertex {

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();
  };

}