#pragma once


#include <vector>

#include "render-vulkan/common.h"
#include "memory/buffer.h"

namespace wage {
  namespace render {
    
    class Device;
    class SwapChain;
    class RenderPass;

    class Pipeline {

    public:
      Pipeline(Device* device, SwapChain* swapChain);

      void create(RenderPass* renderPass);

      void cleanup();

      void bind(VkCommandBuffer commandBuffer);

      VkPipeline wrapped;
      
      VkPipelineLayout layout;

    protected:

      VkShaderModule createShaderModule(const memory::Buffer& code);

      virtual void createDescriptorSetLayouts() = 0;

      virtual std::vector<VkDescriptorSetLayout> descriptorSetLayouts() = 0;

      virtual void cleanupDescriptorSetLayouts() = 0;

      virtual std::vector<VkPushConstantRange> pushConstantRanges() = 0;

      Device* device;

      SwapChain* swapChain;
      
    };

  }
}