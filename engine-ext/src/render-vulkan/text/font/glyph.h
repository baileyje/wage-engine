#pragma once

#include "render-vulkan/core/common.h"
#include "render-vulkan/core/texture.h"
#include "render-vulkan/core/buffer.h"
#include "render-vulkan/ui/vertex.h"

#include "math/vector.h"

namespace wage::render::vulkan {

  class Device;
  class CommandPool;
  class Pipeline;

  class Glyph {
  public:
    Glyph(math::Vector2 size, math::Vector2 bearing, unsigned int advance);

    void push(Device* device, CommandPool* commandPool, VkDescriptorSetLayout textureLayout, VkDescriptorPool descriptorPool);

    void destroy(Device* device);

    math::Vector2 size;

    math::Vector2 bearing;

    unsigned int advance;

    Texture texture;

    bool pushed = false;

    Buffer vertexBuffer;

    Buffer indexBuffer;

  private:
    void createVertexBuffer(Device* device, CommandPool* commandPool, std::vector<UiVertex> vertices);

    void createIndexBuffer(Device* device, CommandPool* commandPool, std::vector<uint32_t> indices);
  };

  struct GlyphPushData {

    alignas(16) glm::vec2 position;

    alignas(16) glm::vec4 color;
  };
}