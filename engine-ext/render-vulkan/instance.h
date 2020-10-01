#pragma once

#include <vector>
#include "render-vulkan/common.h"

namespace wage::render {

  class Instance {

  public:
    VkInstance wrapped;

    Instance();

    void create(bool enableValidationLayers);

    void destroy();

  private:
    std::vector<const char*> getRequiredExtensions();

    bool checkValidationLayerSupport();

    void setupDebugMessenger();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

  private:
    VkDebugUtilsMessengerEXT debugMessenger;
  };
}