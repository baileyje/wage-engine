#pragma once


#include <vector>

#include "render-vulkan/common.h"
#include "render-vulkan/shader.h"
#include "memory/buffer.h"

namespace wage::render::vulkan {

  class VulkanContext;

  class Pipeline {

  public:
    VkPipeline model;

    VkPipeline ui;

    VkDescriptorSetLayout uboLayout;

    VkDescriptorSetLayout textureLayout;

    VkPipelineLayout layout;

    Pipeline(VulkanContext* context);

    void create();

    void cleanup();

    void bindModel(VkCommandBuffer commandBuffer);

    void bindUi(VkCommandBuffer commandBuffer);

  protected:
    VkShaderModule createShaderModule(const memory::Buffer& code);

    std::vector<Shader> shaderStages();

    void createDescriptorSetLayouts();

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts();

    void cleanupDescriptorSetLayouts();

    std::vector<VkPushConstantRange> pushConstantRanges();

    void setupVertexInputInfo(VkPipelineVertexInputStateCreateInfo* info);

    VulkanContext* context;
  };

}