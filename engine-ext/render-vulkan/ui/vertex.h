#pragma once

#include <array>

#include "math/vector.h"
#include "component/color.h"
#include "render-vulkan/core/common.h"

namespace wage::render::vulkan {

  class UiVertex {
  public:

    math::Vector2 position;

    math::Vector2 textureCoord;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
  };
}