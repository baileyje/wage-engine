#include "render-vulkan/model/mesh.h"

#include "render-vulkan/core/device.h"
#include "render-vulkan/core/command_pool.h"

namespace wage::render::vulkan {

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
  
  void VulkanMesh::bind(VkCommandBuffer commandBuffer) {
    VkBuffer vertexBuffers[] = {vertexBuffer.buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
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
    commandPool->copyBuffer(device->graphicsQueue, stagingBuffer.buffer, vertexBuffer.buffer, bufferSize);
    stagingBuffer.destroy();
  }

  void VulkanMesh::createIndexBuffer(Device* device, CommandPool* commandPool) {
    VkDeviceSize bufferSize = sizeof(meshData->indices[0]) * meshData->indices.size();
    Buffer stagingBuffer;
    device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
    stagingBuffer.fillWith(meshData->indices.data(), (size_t)bufferSize);
    device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &indexBuffer);
    commandPool->copyBuffer(device->graphicsQueue, stagingBuffer.buffer, indexBuffer.buffer, bufferSize);
    stagingBuffer.destroy();
  }
}