#pragma once

#include "render-vulkan/common.h"

namespace wage {
  namespace render {

    class Image {

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
        return vkBindImageMemory(device, image, memory, 0);
      }

      void destroy() {
        if (image) {
          vkDestroyImage(device, image, nullptr);
        }
        if (memory) {
          vkFreeMemory(device, memory, nullptr);
        }
      }

      VkResult createImageView(VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* imageView) {
        return commonCreateImageView(device, image, format, aspectFlags, imageView);
      }

      VkImage image = VK_NULL_HANDLE;
      VkDeviceMemory memory = VK_NULL_HANDLE;
      VkDevice device;
      void* mapped = nullptr;
    };

  }
}