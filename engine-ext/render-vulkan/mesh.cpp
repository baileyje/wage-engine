#include "render-vulkan/mesh.h"

#include "render/mesh/mesh.h"
#include "render-vulkan/device.h"
#include "render-vulkan/command_pool.h"

namespace wage {
  namespace render {

    VulkanMesh::VulkanMesh(Mesh* meshData) : meshData(meshData) {}

    void VulkanMesh::destroy(Device* decice) {
      if (!pushed) return;
      vertexBuffer.destroy();
      indexBuffer.destroy();
      pushed = false;
    }

    bool VulkanMesh::loaded() {
      return meshData && meshData->loaded();
    }

    void VulkanMesh::push(Device* device, CommandPool* commandPool) {
      if (pushed) return;
      createVertexBuffer(device, commandPool);
      createIndexBuffer(device, commandPool);
      pushed = true;
    }

    void VulkanMesh::createVertexBuffer(Device* device, CommandPool* commandPool) {
      VkDeviceSize bufferSize = sizeof(meshData->vertices[0]) * meshData->vertices.size();
      Buffer stagingBuffer;
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
      stagingBuffer.fillWith(meshData->vertices.data(), (size_t)bufferSize);
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &vertexBuffer);
      copyBuffer(device->logical, commandPool->wrapped, device->graphicsQueue, stagingBuffer.buffer, vertexBuffer.buffer, bufferSize);
      stagingBuffer.destroy();
    }

    void VulkanMesh::createIndexBuffer(Device* device, CommandPool* commandPool) {
      VkDeviceSize bufferSize = sizeof(meshData->indices[0]) * meshData->indices.size();
      Buffer stagingBuffer;
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
      stagingBuffer.fillWith(meshData->indices.data(), (size_t)bufferSize);
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &indexBuffer);
      copyBuffer(device->logical, commandPool->wrapped, device->graphicsQueue, stagingBuffer.buffer, indexBuffer.buffer, bufferSize);
      stagingBuffer.destroy();
    }
  }
}