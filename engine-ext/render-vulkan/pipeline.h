#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


#include "render-vulkan/device.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/shader.h"
#include "render-vulkan/vertex.h"


namespace wage {
  namespace render {

    class Pipeline {

    public:
      Pipeline(Device* device, SwapChain* swapChain);

      void create();
  
      void cleanup();

      VkPipeline wrapped;

      VkRenderPass renderPass;
      
      VkPipelineLayout layout;

    protected:

      VkShaderModule createShaderModule(const memory::Buffer& code);

      void createRenderPass();

      virtual void createDescriptorSetLayouts() = 0;

      virtual std::vector<VkDescriptorSetLayout> descriptorSetLayouts() = 0;

      virtual void cleanupDescriptorSetLayouts() = 0;

      virtual std::vector<VkPushConstantRange> pushConstantRanges() = 0;

      Device* device;

      SwapChain* swapChain;
      
    };

  }
}