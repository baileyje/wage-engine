#pragma once

#include "render-vulkan/core/common.h"
#include "render-vulkan/core/device.h"
#include "render-vulkan/core/command_pool.h"
#include "render-vulkan/core/buffer.h"
#include "render-vulkan/core/pipeline.h"

namespace wage::render::vulkan {

  class VulkanScene {
  public:
    std::vector<Buffer> modelUniformBuffers;

    std::vector<Buffer> uiUniformBuffers;

    VkDescriptorPool descriptorPool;

    std::vector<VkDescriptorSet> modelDescriptorSets;

    std::vector<VkDescriptorSet> uiDescriptorSets;

    void create(Device* device, CommandPool* commandPool, Pipeline* modelPipeline, Pipeline* textPipeline, int imageCount);

    void destroy(Device* device);


    private:

    void createUniformBuffers(Device* device, int imageCount);

    void createDescriptorPool(Device* device, int imageCount);

    void createDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* modelPipeline, Pipeline* textPipeline, int imageCount);
    
    void createModelDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* modelPipeline, int imageCount);
    
    void createUiDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* textPipeline, int imageCount);
  };

}
