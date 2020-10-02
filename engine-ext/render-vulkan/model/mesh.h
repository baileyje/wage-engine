#pragma once

#include <stdexcept>

#include "render/mesh/mesh.h"
#include "render-vulkan/core/common.h"
#include "render-vulkan/core/buffer.h"

namespace wage::render::vulkan {

  class Device;
  class CommandPool;
  class Buffer;

  class VulkanMesh {
  public:
    Buffer vertexBuffer;

    Buffer indexBuffer;

    Mesh* meshData = nullptr;

    VulkanMesh(Mesh* meshData);

    void destroy(Device* decice);

    bool loaded();

    void push(Device* device, CommandPool* commandPool);

    void createVertexBuffer(Device* device, CommandPool* commandPool);

    void createIndexBuffer(Device* device, CommandPool* commandPool);

  private:
    bool pushed = false;
  };
}