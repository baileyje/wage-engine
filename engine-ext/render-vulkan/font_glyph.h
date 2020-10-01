#pragma once

#include "render-vulkan/common.h"
#include "render-vulkan/texture.h"
#include "render-vulkan/buffer.h"
#include "render-vulkan/vertex.h"

#include "math/vector.h"

namespace wage::render {

  class Device;
  class CommandPool;
  class Pipeline;

  class Glyph {
  public:
    Glyph(math::Vector2 size, math::Vector2 bearing, unsigned int advance);

    void push(Device* device, CommandPool* commandPool, Pipeline* pipeline, VkDescriptorPool descriptorPool);

    void destroy(Device* device);

    math::Vector2 size;

    math::Vector2 bearing;

    unsigned int advance;

    Texture texture;

    bool pushed = false;

    Buffer vertexBuffer;

    Buffer indexBuffer;

  private:
    void createVertexBuffer(Device* device, CommandPool* commandPool, std::vector<Vertex> vertices);

    void createIndexBuffer(Device* device, CommandPool* commandPool, std::vector<uint32_t> indices);
  };
}