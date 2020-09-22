#pragma once

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render-vulkan/common.h"
#include "render-vulkan/instance.h"

namespace wage {
  namespace render {

    class Surface {
    public:
      Surface(Instance* instance) : instance(instance) {}

      inline void create(GLFWwindow* window) {
        if (glfwCreateWindowSurface(instance->wrapped(), window, nullptr, &_wrapped) != VK_SUCCESS) {
          throw std::runtime_error("failed to create window surface!");
        }
      }

      inline void destroy() {
        vkDestroySurfaceKHR(instance->wrapped(), _wrapped, nullptr);
      }

      inline VkSurfaceKHR wrapped() {
        return _wrapped;
      }
    private:
      Instance* instance;
      VkSurfaceKHR _wrapped;
    };
  }
}