#include "render-vulkan/common.h"

#include <vector>

namespace wage {

  namespace render {

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
      QueueFamilyIndices indices;
      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
      int i = 0;
      for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          indices.graphicsFamily = i;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
          indices.presentFamily = i;
        }
        if (indices.isComplete()) {
          break;
        }
        i++;
      }
      return indices;
    }

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
      SwapChainSupportDetails details;
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
      uint32_t formatCount;
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
      if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
      }
      uint32_t presentModeCount;
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
      if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
      }
      return details;
    }

    VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool) {
      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = commandPool;
      allocInfo.commandBufferCount = 1;

      VkCommandBuffer commandBuffer;
      vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      vkBeginCommandBuffer(commandBuffer, &beginInfo);

      return commandBuffer;
    }

    void endSingleTimeCommands(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer) {
      vkEndCommandBuffer(commandBuffer);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &commandBuffer;

      vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(graphicsQueue);

      vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }

    void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

      VkBufferCopy copyRegion{};
      copyRegion.size = size;
      vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

      endSingleTimeCommands(device, commandPool, graphicsQueue, commandBuffer);
    }

    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(device, &memProperties);
      for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
          return i;
        }
      }
      throw std::runtime_error("failed to find suitable memory type!");
    }

    // void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    //   VkImageCreateInfo imageInfo{};
    //   imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    //   imageInfo.imageType = VK_IMAGE_TYPE_2D;
    //   imageInfo.extent.width = width;
    //   imageInfo.extent.height = height;
    //   imageInfo.extent.depth = 1;
    //   imageInfo.mipLevels = 1;
    //   imageInfo.arrayLayers = 1;
    //   imageInfo.format = format;
    //   imageInfo.tiling = tiling;
    //   imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //   imageInfo.usage = usage;
    //   imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    //   imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    //   if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to create image!");
    //   }

    //   VkMemoryRequirements memRequirements;
    //   vkGetImageMemoryRequirements(device, image, &memRequirements);

    //   VkMemoryAllocateInfo allocInfo{};
    //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    //   allocInfo.allocationSize = memRequirements.size;
    //   allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

    //   if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to allocate image memory!");
    //   }
    //   vkBindImageMemory(device, image, imageMemory, 0);
    // }

    VkResult commonCreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* imageView) {
      VkImageViewCreateInfo viewInfo{};
      viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewInfo.image = image;
      viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format = format;
      viewInfo.subresourceRange.aspectMask = aspectFlags;
      viewInfo.subresourceRange.baseMipLevel = 0;
      viewInfo.subresourceRange.levelCount = 1;
      viewInfo.subresourceRange.baseArrayLayer = 0;
      viewInfo.subresourceRange.layerCount = 1;
      return vkCreateImageView(device, &viewInfo, nullptr, imageView);
    }

    VkFormat findDepthFormat(VkPhysicalDevice physicalDevice) {
      return findSupportedFormat(
          physicalDevice,
          {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
          VK_IMAGE_TILING_OPTIMAL,
          VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
      for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
          return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
          return format;
        }
      }

      throw std::runtime_error("failed to find supported format!");
    }
  }
}
