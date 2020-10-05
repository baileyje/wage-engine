#include "render-vulkan/scene.h"
#include "render-vulkan/ubo_scene.h"
#include "render-vulkan/ui/ubo.h"


#include <array>

namespace wage::render::vulkan {

  void VulkanScene::create(Device* device, CommandPool* commandPool, Pipeline* modelPipeline, Pipeline* textPipeline, int imageCount) {
    createUniformBuffers(device, imageCount);
    createDescriptorPool(device, imageCount);
    createDescriptorSets(device, commandPool, modelPipeline, textPipeline, imageCount);
  }

  void VulkanScene::destroy(Device* device) {
    vkDestroyDescriptorPool(device->logical, descriptorPool, nullptr);
    for (auto& buffer : modelUniformBuffers) {
      buffer.destroy();
    }
    for (auto& buffer : uiUniformBuffers) {
      buffer.destroy();
    }
  }

  void VulkanScene::createUniformBuffers(Device* device, int imageCount) {
    VkDeviceSize bufferSize = sizeof(UniformBufferScene);
    modelUniformBuffers.resize(imageCount);
    uiUniformBuffers.resize(imageCount);
    for (size_t i = 0; i < imageCount; i++) {
      device->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &modelUniformBuffers[i]);
      device->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &uiUniformBuffers[i]);
    }
  }

  void VulkanScene::createDescriptorPool(Device* device, int imageCount) {
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(imageCount * 2);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // TODO: Do we have a max number of textures to load....
    poolSizes[1].descriptorCount = static_cast<uint32_t>(imageCount * 100);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(imageCount * 2);

    if (vkCreateDescriptorPool(device->logical, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor pool!");
    }
  }

  void VulkanScene::createModelDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* modelPipeline, int imageCount) {
    std::vector<VkDescriptorSetLayout> modelLayouts(imageCount, modelPipeline->uboLayout);
    VkDescriptorSetAllocateInfo modelAllocInfo{};
    modelAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    modelAllocInfo.descriptorPool = descriptorPool;
    modelAllocInfo.descriptorSetCount = static_cast<uint32_t>(imageCount);
    modelAllocInfo.pSetLayouts = modelLayouts.data();

    modelDescriptorSets.resize(imageCount);
    if (vkAllocateDescriptorSets(device->logical, &modelAllocInfo, modelDescriptorSets.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate descriptor sets!");
    }
    for (size_t i = 0; i < imageCount; i++) {
      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = modelUniformBuffers[i].buffer;
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferScene);

      std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
      descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[0].dstSet = modelDescriptorSets[i];
      descriptorWrites[0].dstBinding = 0;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      vkUpdateDescriptorSets(device->logical, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
  }

  void VulkanScene::createUiDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* textPipeline, int imageCount) {
    std::vector<VkDescriptorSetLayout> uiLayouts(imageCount, textPipeline->uboLayout);
    VkDescriptorSetAllocateInfo uiAllocInfo{};
    uiAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    uiAllocInfo.descriptorPool = descriptorPool;
    uiAllocInfo.descriptorSetCount = static_cast<uint32_t>(imageCount);
    uiAllocInfo.pSetLayouts = uiLayouts.data();

    uiDescriptorSets.resize(imageCount);
    if (vkAllocateDescriptorSets(device->logical, &uiAllocInfo, uiDescriptorSets.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate descriptor sets!");
    }
    for (size_t i = 0; i < imageCount; i++) {
      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = uiUniformBuffers[i].buffer;
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UiUniformBuffer);

      std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
      descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[0].dstSet = uiDescriptorSets[i];
      descriptorWrites[0].dstBinding = 0;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      vkUpdateDescriptorSets(device->logical, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
  }

  void VulkanScene::createDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* modelPipeline, Pipeline* textPipeline, int imageCount) {
    createModelDescriptorSets(device, commandPool, modelPipeline, imageCount);
    createUiDescriptorSets(device, commandPool, textPipeline, imageCount);
  }
}