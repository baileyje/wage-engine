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

    class Device;
    class Surface;

    class SwapChain {

    public:
      VkSwapchainKHR wrapped;

      std::vector<VkImage> images;
      
      VkFormat imageFormat;
      
      VkExtent2D extent;
      
      std::vector<VkImageView> imageViews;
      
      std::vector<VkFramebuffer> frameBuffers;

      SwapChain(Device* device);

      void create(float width, float height, Surface& surface);

      void createFrameBuffers(VkRenderPass renderPass);

      void createDepthResources();

      void cleanup();

    private:
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

      VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

      VkExtent2D chooseSwapExtent(float width, float height, const VkSurfaceCapabilitiesKHR& capabilities);

      void createImageViews();

      Device* device;

      Image depthImage;

      VkImageView depthImageView;
    };
  }
}