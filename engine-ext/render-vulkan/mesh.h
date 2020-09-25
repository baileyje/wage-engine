#pragma once

#include <stdexcept>

#include "render/mesh/mesh.h"
#include "render-vulkan/common.h"
#include "render-vulkan/device.h"
#include "render-vulkan/buffer.h"
#include "render-vulkan/command_pool.h"

namespace wage {
  namespace render {

    class VulkanMesh {
    public:
      VulkanMesh(Mesh* meshData) : meshData(meshData) {}

      void destroy() {}

      bool loaded() {
        return meshData && meshData->loaded();
      }

      void push(Device* device, CommandPool* commandPool) {
        if (pushed) return;
        createVertexBuffer(device, commandPool);
        createIndexBuffer(device, commandPool);
        pushed = true;
      }

      void createVertexBuffer(Device* device, CommandPool* commandPool) {
        VkDeviceSize bufferSize = sizeof(meshData->vertices[0]) * meshData->vertices.size();
        Buffer stagingBuffer;
        device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
        stagingBuffer.fillWith(meshData->vertices.data(), (size_t)bufferSize);
        device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &vertexBuffer);
        copyBuffer(device->logical(), commandPool->wrapped, device->graphicsQueue(), stagingBuffer.buffer, vertexBuffer.buffer, bufferSize);
        stagingBuffer.destroy();
      }

      void createIndexBuffer(Device* device, CommandPool* commandPool) {
        VkDeviceSize bufferSize = sizeof(meshData->indices[0]) * meshData->indices.size();
        Buffer stagingBuffer;
        device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
        stagingBuffer.fillWith(meshData->indices.data(), (size_t)bufferSize);
        device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &indexBuffer);
        copyBuffer(device->logical(), commandPool->wrapped, device->graphicsQueue(), stagingBuffer.buffer, indexBuffer.buffer, bufferSize);
        stagingBuffer.destroy();
      }

      bool pushed = false;

      Buffer vertexBuffer;
      Buffer indexBuffer;
      Mesh* meshData = nullptr;
    };
  }
}