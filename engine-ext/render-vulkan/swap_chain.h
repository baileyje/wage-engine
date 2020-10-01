#pragma once

#include <vector>
#include <array>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "math/vector.h"

#include "render-vulkan/common.h"
#include "render-vulkan/image.h"

namespace wage {
  namespace render {

    class VulkanContext;

    class SwapChain {

    public:
      VkSwapchainKHR wrapped;

      std::vector<VkImage> images;
      
      VkFormat imageFormat;
      
      VkExtent2D extent;
      
      std::vector<VkImageView> imageViews;
      
      std::vector<VkFramebuffer> frameBuffers;

      SwapChain(VulkanContext* context);

      void create(float width, float height);

      void createFrameBuffers();

      void createDepthResources();

      void cleanup();

    private:
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

      VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

      VkExtent2D chooseSwapExtent(float width, float height, const VkSurfaceCapabilitiesKHR& capabilities);

      void createImageViews();

      VulkanContext* context;

      Image depthImage;

      VkImageView depthImageView;
    };
  }
}