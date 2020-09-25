#include "render-vulkan/image.h"

namespace wage {
  namespace render {

    VkResult Image::map(VkDeviceSize size, VkDeviceSize offset) {
      return vkMapMemory(device, memory, offset, size, 0, &mapped);
    }

    void Image::unmap() {
      if (mapped) {
        vkUnmapMemory(device, memory);
        mapped = nullptr;
      }
    }

    void Image::fillWith(void* data, VkDeviceSize size) {
      map();
      memcpy(mapped, data, size);
      unmap();
    }

    VkResult Image::bind(VkDeviceSize offset) {
      return vkBindImageMemory(device, image, memory, 0);
    }

    void Image::destroy() {
      if (image) {
        vkDestroyImage(device, image, nullptr);
      }
      if (memory) {
        vkFreeMemory(device, memory, nullptr);
      }
    }

    VkResult Image::createImageView(VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* imageView) {
      return commonCreateImageView(device, image, format, aspectFlags, imageView);
    }
  }
}