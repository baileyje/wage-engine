#pragma once

#include <vector>
#include <array>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render-vulkan/common.h"
#include "render-vulkan/device.h"
#include "platform/window.h"

namespace wage {
  namespace render {

    class SwapChain {

    public:
      SwapChain(Device* device) : device(device) { }

      void create(platform::Window* window, Surface& surface) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device->physical(), surface.wrapped());
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(window, swapChainSupport.capabilities);
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
          imageCount = swapChainSupport.capabilities.maxImageCount;
        }
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface.wrapped();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        QueueFamilyIndices indices = findQueueFamilies(device->physical(), surface.wrapped());
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
        if (vkCreateSwapchainKHR(device->logical(), &createInfo, nullptr, &_wrapped) != VK_SUCCESS) {
          throw std::runtime_error("failed to create swap chain!");
        }
        vkGetSwapchainImagesKHR(device->logical(), _wrapped, &imageCount, nullptr);
        _images.resize(imageCount);
        vkGetSwapchainImagesKHR(device->logical(), _wrapped, &imageCount, _images.data());
        _imageFormat = surfaceFormat.format;
        _extent = extent;
        createImageViews();
      }

      void createFrameBuffers(VkRenderPass renderPass) {
        _frameBuffers.resize(_imageViews.size());
        for (size_t i = 0; i < _imageViews.size(); i++) {
          std::array<VkImageView, 2> attachments = {
              _imageViews[i], depthImageView};

          VkFramebufferCreateInfo framebufferInfo{};
          framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
          framebufferInfo.renderPass = renderPass;
          framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
          framebufferInfo.pAttachments = attachments.data();
          framebufferInfo.width = _extent.width;
          framebufferInfo.height = _extent.height;
          framebufferInfo.layers = 1;

          if (vkCreateFramebuffer(device->logical(), &framebufferInfo, nullptr, &_frameBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
          }
        }
      }

      void createDepthResources() {
        VkFormat depthFormat = findDepthFormat(device->physical());
        device->createImage(_extent.width, _extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &depthImage);
        depthImage.createImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, &depthImageView);
      }

      void cleanup() {
        for (auto imageView : _imageViews) {
          vkDestroyImageView(device->logical(), imageView, nullptr);
        }
      }

      void cleanupFrameBuffers() {
        vkDestroyImageView(device->logical(), depthImageView, nullptr);
        depthImage.destroy();
        for (auto framebuffer : _frameBuffers) {
          vkDestroyFramebuffer(device->logical(), framebuffer, nullptr);
        }
      }

      inline VkSwapchainKHR wrapped() {
        return _wrapped;
      }

      inline std::vector<VkImage>& images() {
        return _images;
      }

      inline VkFormat imageFormat() {
        return _imageFormat;
      }

      inline VkExtent2D extent() {
        return _extent;
      }

      inline std::vector<VkFramebuffer>& frameBuffers() {
        return _frameBuffers;
      }

    private:
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
          if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
          }
        }
        return availableFormats[0];
      }

      VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
          if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
          }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
      }

      VkExtent2D chooseSwapExtent(platform::Window* window, const VkSurfaceCapabilitiesKHR& capabilities) {
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

      void createImageViews() {
        _imageViews.resize(_images.size());
        for (size_t i = 0; i < _images.size(); i++) {
          commonCreateImageView(device->logical(), _images[i], _imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, &_imageViews[i]);
        }
      }

      Device* device;

      VkSwapchainKHR _wrapped;
      std::vector<VkImage> _images;
      VkFormat _imageFormat;
      VkExtent2D _extent;
      std::vector<VkImageView> _imageViews;
      std::vector<VkFramebuffer> _frameBuffers;

      Image depthImage;
      VkImageView depthImageView;
    };

  }
}