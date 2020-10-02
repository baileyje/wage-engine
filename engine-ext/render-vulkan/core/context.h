#pragma once

#include "platform/window.h"

#include "render-vulkan/core/command_pool.h"
#include "render-vulkan/core/instance.h"
#include "render-vulkan/core/surface.h"
#include "render-vulkan/core/device.h"
#include "render-vulkan/core/swap_chain.h"
#include "render-vulkan/core/render_pass.h"

namespace wage::render::vulkan {

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