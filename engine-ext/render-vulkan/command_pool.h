#pragma once

#include "render-vulkan/common.h"


namespace wage {
  namespace render {

    class Surface;
    class Device;
    class SwapChain;
    class ModelPipeline;

    class CommandPool {

    public:
      
      VkCommandPool wrapped;
      
      std::vector<VkCommandBuffer> commandBuffers;  

      CommandPool(Device* device, SwapChain* swapChain);

      void create(Surface& surface, ModelPipeline* pipeline);

      void destroy();

      void cleanupBuffers();

      void beginCommandBuffer(VkCommandBuffer commandBuffer, ModelPipeline* pipeline, uint32_t imageIndex);

      void endCommandBuffer(VkCommandBuffer commandBuffer);

    private:
      
      void createCommandBuffers();

      Device* device;
      
      SwapChain* swapChain;
    };

  }
}