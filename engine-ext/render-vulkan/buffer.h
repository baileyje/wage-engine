#pragma once

#include "render-vulkan/common.h"

namespace wage {
  namespace render {

    class Buffer {

    public:
      inline VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
        return vkMapMemory(device, memory, offset, size, 0, &mapped);
      }

      inline void unmap() {
        if (mapped) {
          vkUnmapMemory(device, memory);
          mapped = nullptr;
        }
      }

      void fillWith(void* data, VkDeviceSize size) {
        map();
        memcpy(mapped, data, size);
        unmap();
      }

      inline VkResult bind(VkDeviceSize offset = 0) {
        return vkBindBufferMemory(device, buffer, memory, offset);
      }

      void destroy() {
        if (buffer) {
          vkDestroyBuffer(device, buffer, nullptr);
        }
        if (memory) {
          vkFreeMemory(device, memory, nullptr);
        }
      }

      void setupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
        descriptor.offset = offset;
        descriptor.buffer = buffer;
        descriptor.range = size;
      }

      VkBuffer buffer = VK_NULL_HANDLE;
      VkDeviceMemory memory = VK_NULL_HANDLE;
      VkDevice device;
      VkDescriptorBufferInfo descriptor;
      VkBufferUsageFlags usageFlags;
      VkMemoryPropertyFlags memoryPropertyFlags;
      VkDeviceSize size = 0;
      VkDeviceSize alignment = 0;
      void* mapped = nullptr;


    };

  }
}