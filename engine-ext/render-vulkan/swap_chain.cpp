#include "render-vulkan/swap_chain.h"

#include "render-vulkan/device.h"
#include "render-vulkan/surface.h"

namespace wage {
  namespace render {

    SwapChain::SwapChain(Device* device) : device(device) {}

    void SwapChain::create(platform::Window* window, Surface& surface) {
      SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device->physical, surface.wrapped);
      VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
      VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
      extent = chooseSwapExtent(window, swapChainSupport.capabilities);
      uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
      if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
      }
      VkSwapchainCreateInfoKHR createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      createInfo.surface = surface.wrapped;
      createInfo.minImageCount = imageCount;
      createInfo.imageFormat = surfaceFormat.format;
      createInfo.imageColorSpace = surfaceFormat.colorSpace;
      createInfo.imageExtent = extent;
      createInfo.imageArrayLayers = 1;
      createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
      QueueFamilyIndices indices = findQueueFamilies(device->physical, surface.wrapped);
      uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
      if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
      } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      }
      createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
      createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      createInfo.presentMode = presentMode;
      createInfo.clipped = VK_TRUE;
      createInfo.oldSwapchain = VK_NULL_HANDLE;
      if (vkCreateSwapchainKHR(device->logical, &createInfo, nullptr, &wrapped) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
      }
      vkGetSwapchainImagesKHR(device->logical, wrapped, &imageCount, nullptr);
      images.resize(imageCount);
      vkGetSwapchainImagesKHR(device->logical, wrapped, &imageCount, images.data());
      imageFormat = surfaceFormat.format;
      createImageViews();
    }

    void SwapChain::createFrameBuffers(VkRenderPass renderPass) {
      frameBuffers.resize(imageViews.size());
      for (size_t i = 0; i < imageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            imageViews[i], depthImageView};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device->logical, &framebufferInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS) {
          throw std::runtime_error("failed to create framebuffer!");
        }
      }
    }

    void SwapChain::createDepthResources() {
      VkFormat depthFormat = findDepthFormat(device->physical);
      device->createImage(extent.width, extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &depthImage);
      depthImage.createImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, &depthImageView);
    }

    void SwapChain::cleanup() {
      vkDestroyImageView(device->logical, depthImageView, nullptr);
      depthImage.destroy();
      for (auto framebuffer : frameBuffers) {
        vkDestroyFramebuffer(device->logical, framebuffer, nullptr);
      }
      for (auto imageView : imageViews) {
        vkDestroyImageView(device->logical, imageView, nullptr);
      }
      vkDestroySwapchainKHR(device->logical, wrapped, nullptr);
    }

    VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
      for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
          return availableFormat;
        }
      }
      return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
      for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
          return availablePresentMode;
        }
      }
      return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::chooseSwapExtent(platform::Window* window, const VkSurfaceCapabilitiesKHR& capabilities) {
      if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
      } else {
        // TODO: Get from window.....
        VkExtent2D actualExtent = {static_cast<uint32_t>(window->width()), static_cast<uint32_t>(window->height())};
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
      }
    }

    void SwapChain::createImageViews() {
      imageViews.resize(images.size());
      for (size_t i = 0; i < images.size(); i++) {
        commonCreateImageView(device->logical, images[i], imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, &imageViews[i]);
      }
    }
  }
}