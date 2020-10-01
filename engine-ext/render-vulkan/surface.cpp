#include "render-vulkan/surface.h"

#include "render-vulkan/instance.h"

namespace wage::render {

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