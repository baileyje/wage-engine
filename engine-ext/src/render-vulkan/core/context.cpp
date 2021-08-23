#include "render-vulkan/core/context.h"

namespace wage::render::vulkan {
  VulkanContext::VulkanContext() : surface(&instance), swapChain(this), renderPass(&device, &swapChain) {
  }

  void VulkanContext::create(platform::Window* window) {
    auto glfwWindow = window->as<GLFWwindow>();
    instance.create(enableValidationLayers);
    surface.create(glfwWindow);
    device.create(instance.wrapped, &surface);
    swapChain.create(window->width(), window->height());
    frameCount = swapChain.images.size();
    renderPass.create();
    swapChain.createDepthResources();
    swapChain.createFrameBuffers();
  }

  void VulkanContext::destroy() {
    renderPass.destroy();
    swapChain.cleanup();
    device.destroy();
    surface.destroy();
    instance.destroy();
  }
}