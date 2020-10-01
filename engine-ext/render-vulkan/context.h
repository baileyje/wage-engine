#pragma once

#include "platform/window.h"

#include "render-vulkan/command_pool.h"
#include "render-vulkan/instance.h"
#include "render-vulkan/surface.h"
#include "render-vulkan/device.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/render_pass.h"

namespace wage::render {

  class VulkanContext {
  public:
    Instance instance;

    Surface surface;

    Device device;

    SwapChain swapChain;

    RenderPass renderPass;

    int frameCount;

    VulkanContext();

    void create(platform::Window* window);

    void destroy();
  };
}