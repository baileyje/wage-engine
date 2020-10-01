#pragma once

#include "render-vulkan/common.h"

namespace wage::render {

  class Image {

  public:
    VkImage image = VK_NULL_HANDLE;

    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkDevice device;

    void* mapped = nullptr;

    VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    void unmap();

    void fillWith(void* data, VkDeviceSize size);

    VkResult bind(VkDeviceSize offset = 0);

    void destroy();

    VkResult createImageView(VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* imageView);
  };

}