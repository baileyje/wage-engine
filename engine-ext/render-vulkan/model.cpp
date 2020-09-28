#include "render-vulkan/model.h"
#include "render-vulkan/ubo_scene.h"
#include "render-vulkan/command_pool.h"

namespace wage {
  namespace render {

    void Model::destroy(Device* device) {
      vkDestroyDescriptorPool(device->logical, descriptorPool, nullptr);
      texture->destroy(device);
      mesh->destroy(device);
    }

    void Model::createDescriptorPool(Device* device, int imageCount) {
      std::array<VkDescriptorPoolSize, 2> poolSizes{};
      poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSizes[0].descriptorCount = static_cast<uint32_t>(imageCount);
      poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      poolSizes[1].descriptorCount = static_cast<uint32_t>(imageCount);

      VkDescriptorPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
      poolInfo.pPoolSizes = poolSizes.data();
      poolInfo.maxSets = static_cast<uint32_t>(imageCount);

      if (vkCreateDescriptorPool(device->logical, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
      }
    }

    void Model::createDescriptorSets(Device* device, CommandPool* commandPool, ModelPipeline* pipeline, int imageCount) {
      std::vector<VkDescriptorSetLayout> layouts(imageCount, pipeline->modelTextureLayout);
      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptorPool;
      allocInfo.descriptorSetCount = static_cast<uint32_t>(imageCount);
      allocInfo.pSetLayouts = layouts.data();

      descriptorSets.resize(imageCount);
      if (vkAllocateDescriptorSets(device->logical, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
      }

      for (size_t i = 0; i < imageCount; i++) {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture->imageView;
        imageInfo.sampler = texture->sampler;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device->logical, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
      }
    }
  }
}