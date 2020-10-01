#pragma once

#include "render-vulkan/common.h"

namespace wage::render::vulkan {

  class Surface;
  class Image;
  class Buffer;

  class Device {

  public:
    VkPhysicalDevice physical = VK_NULL_HANDLE;

    VkDevice logical = VK_NULL_HANDLE;

    VkQueue graphicsQueue;

    VkQueue presentQueue;

    void create(VkInstance instance, Surface* surface);

    void destroy();

    VkResult createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size, Buffer* buffer);

    VkResult createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, Image* image);

  private:
    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

    void createLogicalDevice(VkSurfaceKHR surface);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  };

}
