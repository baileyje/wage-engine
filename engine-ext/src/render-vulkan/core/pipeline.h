#pragma once


#include <vector>

#include "render-vulkan/core/common.h"
#include "render-vulkan/core/shader.h"
#include "memory/buffer.h"

namespace wage::render::vulkan {

  class VulkanContext;

  class Pipeline {

  public:
    VkPipeline wrapped;

    VkPipelineLayout layout;

    VkDescriptorSetLayout uboLayout;

    Pipeline(VulkanContext* context);

    virtual void create() = 0;

    virtual void cleanup();

    void bind(VkCommandBuffer commandBuffer);

  protected:

    VkShaderModule createShaderModule(const memory::Buffer& code);

    VulkanContext* context;
  };

}