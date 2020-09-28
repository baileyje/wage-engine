#pragma once

#include "render-vulkan/common.h"


namespace wage {
  namespace render {

    class VulkanContext;
    
    class CommandPool {

    public:
      
      VkCommandPool wrapped;
      
      std::vector<VkCommandBuffer> commandBuffers;

      CommandPool(VulkanContext* context);

      void create();

      void destroy();

      void cleanupBuffers();

      VkCommandBuffer beginCommandBuffer(uint32_t imageIndex);

      void endCommandBuffer(VkCommandBuffer commandBuffer);

    private:
      
      void createCommandBuffers();

      VulkanContext* context;
      
    };

  }
}