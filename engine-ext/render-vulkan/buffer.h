#pragma once

#include "render-vulkan/common.h"

namespace wage {
  namespace render {

    class Buffer {

    public:
      VkBuffer buffer = VK_NULL_HANDLE;
      
      VkDeviceMemory memory = VK_NULL_HANDLE;
      
      VkDevice device;
      
      VkDescriptorBufferInfo descriptor;
      
      VkBufferUsageFlags usageFlags;
      
      VkMemoryPropertyFlags memoryPropertyFlags;
      
      VkDeviceSize size = 0;
      
      VkDeviceSize alignment = 0;
      
      void* mapped = nullptr;

      VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

      void unmap();

      void fillWith(void* data, VkDeviceSize size);

      VkResult bind(VkDeviceSize offset = 0);
      
      void destroy();

      void setupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    };

  }
}