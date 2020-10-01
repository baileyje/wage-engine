#include "render-vulkan/font_glyph.h"

#include "render-vulkan/device.h"
#include "render-vulkan/command_pool.h"
#include "render-vulkan/pipeline.h"

namespace wage::render {
  
  Glyph::Glyph(math::Vector2 size, math::Vector2 bearing, unsigned int advance) : size(size), bearing(bearing), advance(advance), texture(VK_FORMAT_R8_UNORM), pushed(false) {}

  void Glyph::push(Device* device, CommandPool* commandPool, Pipeline* pipeline, VkDescriptorPool descriptorPool) {
    if (pushed) {
      return;
    }
    if (texture.imageSize > 0) {
      texture.push(device, commandPool, pipeline, descriptorPool, commandPool->commandBuffers.size());
    }
    // Need to tighten this up big time
    std::vector<Vertex> vertices = {
        {{size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}}, // top right
        {{size.x, 0.0, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},    // bottom right
        {{0.0, 0.0, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},       // bottom left
        {{0.0, size.y, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}     // top left
    };
    std::vector<uint32_t> indices = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    createVertexBuffer(device, commandPool, vertices);
    createIndexBuffer(device, commandPool, indices);
    pushed = true;
  }

  void Glyph::destroy(Device* device) {
    if (!pushed) return;
    texture.destroy(device);
    vertexBuffer.destroy();
    indexBuffer.destroy();
    pushed = false;
  }

  void Glyph::createVertexBuffer(Device* device, CommandPool* commandPool, std::vector<Vertex> vertices) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    Buffer stagingBuffer;
    device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
    stagingBuffer.fillWith(vertices.data(), (size_t)bufferSize);
    device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &vertexBuffer);
    commandPool->copyBuffer(device->graphicsQueue, stagingBuffer.buffer, vertexBuffer.buffer, bufferSize);
    stagingBuffer.destroy();
  }

  void Glyph::createIndexBuffer(Device* device, CommandPool* commandPool, std::vector<uint32_t> indices) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    Buffer stagingBuffer;
    device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
    stagingBuffer.fillWith(indices.data(), (size_t)bufferSize);
    device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &indexBuffer);
    commandPool->copyBuffer(device->graphicsQueue, stagingBuffer.buffer, indexBuffer.buffer, bufferSize);
    stagingBuffer.destroy();
  }

}