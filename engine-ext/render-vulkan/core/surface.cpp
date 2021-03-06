#include "render-vulkan/core/surface.h"

#include "render-vulkan/core/instance.h"

namespace wage::render::vulkan {

  Surface::Surface(Instance* instance) : instance(instance) {}

  void Surface::create(GLFWwindow* window) {
    if (glfwCreateWindowSurface(instance->wrapped, window, nullptr, &wrapped) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface!");
    }
  }

  void Surface::destroy() {
    vkDestroySurfaceKHR(instance->wrapped, wrapped, nullptr);
  }
}