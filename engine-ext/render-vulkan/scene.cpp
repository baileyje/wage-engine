#include "render-vulkan/scene.h"
#include "render-vulkan/ubo_scene.h"

namespace wage {
  namespace render {

    void VulkanScene::create(Device* device, CommandPool* commandPool, GraphicsPipeline* pipeline, int imageCount) {
      createUniformBuffers(device, imageCount);
      createDescriptorPool(device, imageCount);
      createDescriptorSets(device, commandPool, pipeline, imageCount);
    }

    void VulkanScene::destroy(Device* device) {
      // vkDestroyDescriptorSetLayout(device->logical(), sceneUboLayout, nullptr);
    }

    void VulkanScene::createUniformBuffers(Device* device, int imageCount) {
      VkDeviceSize bufferSize = sizeof(UniformBufferScene);
      uniformBuffers.resize(imageCount);
      for (size_t i = 0; i < imageCount; i++) {
        device->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &uniformBuffers[i]);
      }
    }

    void VulkanScene::createDescriptorPool(Device* device, int imageCount) {
      std::array<VkDescriptorPoolSize, 1> poolSizes{};
      poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSizes[0].descriptorCount = static_cast<uint32_t>(imageCount);

      VkDescriptorPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
      poolInfo.pPoolSizes = poolSizes.data();
      poolInfo.maxSets = static_cast<uint32_t>(imageCount);

      if (vkCreateDescriptorPool(device->logical(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
      }
    }

    void VulkanScene::createDescriptorSets(Device* device, CommandPool* commandPool, GraphicsPipeline* pipeline, int imageCount) {
      std::vector<VkDescriptorSetLayout> layouts(imageCount, pipeline->sceneUboLayout);
      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptorPool;
      allocInfo.descriptorSetCount = static_cast<uint32_t>(imageCount);
      allocInfo.pSetLayouts = layouts.data();

      descriptorSets.resize(imageCount);
      if (vkAllocateDescriptorSets(device->logical(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
      }
      for (size_t i = 0; i < imageCount; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i].buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferScene);

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device->logical(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
      }
    }
  }
}