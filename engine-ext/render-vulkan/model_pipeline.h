#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <array>

#include "render-vulkan/device.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/shader.h"
#include "render-vulkan/vertex.h"
#include "render-vulkan/pipeline.h"
#include "core/core.h"
#include "fs/file_system.h"
#include "memory/allocator.h"
#include "memory/buffer.h"


namespace wage {
  namespace render {

    class ModelPipeline : public Pipeline {

    public:
      VkDescriptorSetLayout modelTextureLayout;
      VkDescriptorSetLayout sceneUboLayout;

      ModelPipeline(Device * device, SwapChain * swapChain) : Pipeline(device, swapChain) {}

      virtual void createDescriptorSetLayouts() {
        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 0;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        std::array<VkDescriptorSetLayoutBinding,1> samplerBindings = {samplerLayoutBinding};
        VkDescriptorSetLayoutCreateInfo samplerLayoutInfo{};
        samplerLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        samplerLayoutInfo.bindingCount = static_cast<uint32_t>(samplerBindings.size());
        samplerLayoutInfo.pBindings = samplerBindings.data();
        if (vkCreateDescriptorSetLayout(device->logical, &samplerLayoutInfo, nullptr, &modelTextureLayout) != VK_SUCCESS) {
          throw std::runtime_error("failed to create descriptor set layout!");
        }

        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        std::array<VkDescriptorSetLayoutBinding, 1> uboBindings = {uboLayoutBinding};
        VkDescriptorSetLayoutCreateInfo uboLayoutInfo{};
        uboLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        uboLayoutInfo.bindingCount = static_cast<uint32_t>(uboBindings.size());
        uboLayoutInfo.pBindings = uboBindings.data();
        if (vkCreateDescriptorSetLayout(device->logical, &uboLayoutInfo, nullptr, &sceneUboLayout) != VK_SUCCESS) {
          throw std::runtime_error("failed to create descriptor set layout!");
        }
      }

      virtual std::vector<VkDescriptorSetLayout> descriptorSetLayouts() {
        return { sceneUboLayout, modelTextureLayout };
      }

      virtual void cleanupDescriptorSetLayouts(){
        vkDestroyDescriptorSetLayout(device->logical, modelTextureLayout, nullptr);
        vkDestroyDescriptorSetLayout(device->logical, sceneUboLayout, nullptr);
      }

      virtual std::vector<VkPushConstantRange> pushConstantRanges() {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(glm::mat4);
        return { pushConstantRange };
      }
    };

  }
}