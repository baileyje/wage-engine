#pragma once

#include <vector>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render-vulkan/common.h"
#include "render-vulkan/surface.h"
#include "render-vulkan/buffer.h"
#include "render-vulkan/image.h"

namespace wage {
  namespace render {

    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    class Device {

    public:
      void create(VkInstance instance, Surface surface) {
        pickPhysicalDevice(instance, surface.wrapped());
        createLogicalDevice(surface.wrapped());
      }

      void destroy() {
        vkDestroyDevice(_logical, nullptr);
      }

      inline VkPhysicalDevice physical() {
        return _physical;
      }

      inline VkDevice logical() {
        return _logical;
      }

      inline VkQueue graphicsQueue() {
        return _graphicsQueue;
      }

      inline VkQueue presentQueue() {
        return _presentQueue;
      }

      VkResult createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size, Buffer* buffer) {
        buffer->device = _logical;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(_logical, &bufferInfo, nullptr, &buffer->buffer) != VK_SUCCESS) {
          throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(_logical, buffer->buffer, &memRequirements);
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(_physical, memRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(_logical, &allocInfo, nullptr, &buffer->memory) != VK_SUCCESS) {
          throw std::runtime_error("failed to allocate buffer memory!");
        }

        buffer->alignment = memRequirements.alignment;
        buffer->size = size;
        buffer->usageFlags = usage;
        buffer->memoryPropertyFlags = properties;

        buffer->setupDescriptor();
        return buffer->bind();
      }

      VkResult createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, Image* image) {
        image->device = _logical;
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateImage(_logical, &imageInfo, nullptr, &image->image) != VK_SUCCESS) {
          throw std::runtime_error("failed to create image!");
        }
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(_logical, image->image, &memRequirements);
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(_physical, memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(_logical, &allocInfo, nullptr, &image->memory) != VK_SUCCESS) {
          throw std::runtime_error("failed to allocate image memory!");
        }
        return image->bind();
      }

    private:
      void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
          throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        for (const auto& device : devices) {
          if (isDeviceSuitable(device, surface)) {
            _physical = device;
            break;
          }
        }
        if (_physical == VK_NULL_HANDLE) {
          throw std::runtime_error("failed to find a suitable GPU!");
        }
      }

      bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices = findQueueFamilies(device, surface);
        bool extensionsSupported = checkDeviceExtensionSupport(device);
        bool swapChainAdequate = false;
        if (extensionsSupported) {
          SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
          swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }
        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
        return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
      }

      void createLogicalDevice(VkSurfaceKHR surface) {
        QueueFamilyIndices indices = findQueueFamilies(_physical, surface);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
          VkDeviceQueueCreateInfo queueCreateInfo{};
          queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
          queueCreateInfo.queueFamilyIndex = queueFamily;
          queueCreateInfo.queueCount = 1;
          queueCreateInfo.pQueuePriorities = &queuePriority;
          queueCreateInfos.push_back(queueCreateInfo);
        }
        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        if (enableValidationLayers) {
          createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
          createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
          createInfo.enabledLayerCount = 0;
        }
        if (vkCreateDevice(_physical, &createInfo, nullptr, &_logical) != VK_SUCCESS) {
          throw std::runtime_error("failed to create logical device!");
        }
        vkGetDeviceQueue(_logical, indices.graphicsFamily.value(), 0, &_graphicsQueue);
        vkGetDeviceQueue(_logical, indices.presentFamily.value(), 0, &_presentQueue);
      }

      bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
        for (const auto& extension : availableExtensions) {
          requiredExtensions.erase(extension.extensionName);
        }
        return requiredExtensions.empty();
      }

      VkPhysicalDevice _physical = VK_NULL_HANDLE;
      VkDevice _logical;
      VkQueue _graphicsQueue;
      VkQueue _presentQueue;
    };

  }

}