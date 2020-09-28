#include "render-vulkan/context.h"



namespace wage {
  namespace render {
    VulkanContext::VulkanContext() : surface(&instance), swapChain(&device), renderPass(&device, &swapChain) {
    }

    void VulkanContext::create(platform::Window* window) {
      auto glfwWindow = window->as<GLFWwindow>();
      instance.create(enableValidationLayers);
      surface.create(glfwWindow);
      device.create(instance.wrapped, &surface);
      swapChain.create(window->width(), window->height(), surface);
      renderPass.create();
      swapChain.createDepthResources();
      swapChain.createFrameBuffers(renderPass.wrapped);
    }
    
    void VulkanContext::destroy() {
      renderPass.destroy();
      swapChain.cleanup();
      device.destroy();
      surface.destroy();
      instance.destroy();
    }
  }
}