#include "render-vulkan/buffer.h"

namespace wage::render {
  
  VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset) {
    return vkMapMemory(device, memory, offset, size, 0, &mapped);
  }

  void Buffer::unmap() {
    if (mapped) {
      vkUnmapMemory(device, memory);
      mapped = nullptr;
    }
  }

  void Buffer::fillWith(void* data, VkDeviceSize size) {
    map();
    memcpy(mapped, data, size);
    unmap();
  }

  VkResult Buffer::bind(VkDeviceSize offset) {
    return vkBindBufferMemory(device, buffer, memory, offset);
  }

  void Buffer::destroy() {
    if (buffer) {
      vkDestroyBuffer(device, buffer, nullptr);
    }
    if (memory) {
      vkFreeMemory(device, memory, nullptr);
    }
  }

  void Buffer::setupDescriptor(VkDeviceSize size, VkDeviceSize offset) {
    descriptor.offset = offset;
    descriptor.buffer = buffer;
    descriptor.range = size;
  }
}