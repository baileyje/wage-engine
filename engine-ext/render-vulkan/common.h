#pragma once

#include <optional>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace wage {

  namespace render {
    struct QueueFamilyIndices {
      std::optional<uint32_t> graphicsFamily;
      std::optional<uint32_t> presentFamily;

      bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
      }
    };

    struct SwapChainSupportDetails {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
    };

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties);

   VkResult commonCreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* imageView);

    VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    
    VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

    // #ifdef NDEBUG
    const bool enableValidationLayers = true;
    // #else
    //     const bool enableValidationLayers = false;
    // #endif

      }
    }