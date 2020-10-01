#pragma once

#include "render-vulkan/common.h"

namespace wage::render::vulkan {

  class Device;
  class SwapChain;

  class RenderPass {

  public:
    VkRenderPass wrapped;

    RenderPass(Device* device, SwapChain* swapChain);

    void create();

    void destroy();

    void begin(VkCommandBuffer commandBuffer, int indexImage);

    void end(VkCommandBuffer commandBuffer);

  private:
    Device* device;
    SwapChain* swapChain;
  };
}