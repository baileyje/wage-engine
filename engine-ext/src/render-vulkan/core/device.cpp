#include "render-vulkan/core/device.h"

#include <vector>
#include <set>

#include <iostream>

#include "memory/buffer.h"

#include "render-vulkan/core/surface.h"
#include "render-vulkan/core/buffer.h"
#include "render-vulkan/core/image.h"

const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

namespace wage::render::vulkan {

  void Device::create(VkInstance instance, Surface* surface) {
    pickPhysicalDevice(instance, surface->wrapped);
    createLogicalDevice(surface->wrapped);
  }

  void Device::destroy() {
    vkDestroyDevice(logical, nullptr);
  }

  VkResult Device::createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize size, Buffer* buffer) {
    buffer->device = logical;
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(logical, &bufferInfo, nullptr, &buffer->buffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logical, buffer->buffer, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physical, memRequirements.memoryTypeBits, properties);
    if (vkAllocateMemory(logical, &allocInfo, nullptr, &buffer->memory) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate buffer memory!");
    }

    buffer->alignment = memRequirements.alignment;
    buffer->size = size;
    buffer->usageFlags = usage;
    buffer->memoryPropertyFlags = properties;

    buffer->setupDescriptor();
    return buffer->bind();
  }

  VkResult Device::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, Image* image) {
    image->device = logical;
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
    if (vkCreateImage(logical, &imageInfo, nullptr, &image->image) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image!");
    }
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(logical, image->image, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physical, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(logical, &allocInfo, nullptr, &image->memory) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate image memory!");
    }
    return image->bind();
  }

  void Device::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
      throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    for (const auto& device : devices) {
      if (isDeviceSuitable(device, surface)) {
        physical = device;
        break;
      }
    }
    if (physical == VK_NULL_HANDLE) {
      throw std::runtime_error("failed to find a suitable GPU!");
    }
    // VkPhysicalDeviceProperties physicalProperties = {};
    // vkGetPhysicalDeviceProperties(physical, &physicalProperties);
    // fprintf(stdout, "Device Name:    %s\n", physicalProperties.deviceName);
    // fprintf(stdout, "Device Type:    %d\n", physicalProperties.deviceType);
    // fprintf(stdout, "Driver Version: %d\n", physicalProperties.driverVersion);
  }

  bool Device::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
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

  void Device::createLogicalDevice(VkSurfaceKHR surface) {
    QueueFamilyIndices indices = findQueueFamilies(physical, surface);
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
    deviceFeatures.fillModeNonSolid = VK_TRUE;
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
    if (vkCreateDevice(physical, &createInfo, nullptr, &logical) != VK_SUCCESS) {
      throw std::runtime_error("failed to create logical device!");
    }
    vkGetDeviceQueue(logical, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(logical, indices.presentFamily.value(), 0, &presentQueue);
  }

  bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device) {
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
}