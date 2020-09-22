#pragma once

#include <vector>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render-vulkan/common.h"
#include "render-vulkan/surface.h"

namespace wage {
  namespace render {

    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    class Device {

    public:
      void create(VkInstance instance, Surface surface) {
        pickPhysicalDevice(instance, surface.wrapped());
        createLogicalDevice(surface.wrapped());
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