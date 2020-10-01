#pragma once

#include "render-vulkan/common.h"
#include "render-vulkan/device.h"
#include "render-vulkan/command_pool.h"
#include "render-vulkan/buffer.h"
#include "render-vulkan/pipeline.h"

namespace wage::render {

  class VulkanScene {
  public:
    std::vector<Buffer> modelUniformBuffers;

    std::vector<Buffer> uiUniformBuffers;

    VkDescriptorPool descriptorPool;

    std::vector<VkDescriptorSet> modelDescriptorSets;

    std::vector<VkDescriptorSet> uiDescriptorSets;

    void create(Device* device, CommandPool* commandPool, Pipeline* modelPipeline, Pipeline* uiPipeline, int imageCount);

    void destroy(Device* device);

    void createUniformBuffers(Device* device, int imageCount);

    void createDescriptorPool(Device* device, int imageCount);

    void createDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* modelPipeline, Pipeline* uiPipeline, int imageCount);
  };

}
