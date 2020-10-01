// #pragma once

// #define GLFW_INCLUDE_VULKAN
// #include <GLFW/glfw3.h>

// #include <vector>
// #include <array>

// #include "render-vulkan/context.h"
// #include "render-vulkan/device.h"
// #include "render-vulkan/swap_chain.h"
// #include "render-vulkan/shader.h"
// #include "render-vulkan/vertex.h"
// #include "render-vulkan/pipeline.h"
// #include "core/core.h"
// #include "fs/file_system.h"
// #include "memory/allocator.h"
// #include "memory/buffer.h"


// namespace wage {
//   namespace render {

//     class ModelPipeline : public Pipeline {

//     public:
      
//       ModelPipeline(VulkanContext* context) : Pipeline(context) {}

//       virtual std::vector<Shader> shaderStages() {
//         auto fs = core::Core::Instance->get<fs::FileSystem>();
//         Shader vertShader(&context->device, VK_SHADER_STAGE_VERTEX_BIT);
//         vertShader.create(fs->read({{"resources/shader/model.vert.spv"}}, memory::Allocator::Temporary()));
//         Shader fragShader(&context->device, VK_SHADER_STAGE_FRAGMENT_BIT);
//         fragShader.create(fs->read({{"resources/shader/model.frag.spv"}}, memory::Allocator::Temporary()));
//         return { vertShader, fragShader };
//       }

//       virtual void createDescriptorSetLayouts() {
//         VkDescriptorSetLayoutBinding samplerLayoutBinding{};
//         samplerLayoutBinding.binding = 0;
//         samplerLayoutBinding.descriptorCount = 1;
//         samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//         samplerLayoutBinding.pImmutableSamplers = nullptr;
//         samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//         std::array<VkDescriptorSetLayoutBinding,1> samplerBindings = {samplerLayoutBinding};
//         VkDescriptorSetLayoutCreateInfo samplerLayoutInfo{};
//         samplerLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//         samplerLayoutInfo.bindingCount = static_cast<uint32_t>(samplerBindings.size());
//         samplerLayoutInfo.pBindings = samplerBindings.data();
//         if (vkCreateDescriptorSetLayout(context->device.logical, &samplerLayoutInfo, nullptr, &textureLayout) != VK_SUCCESS) {
//           throw std::runtime_error("failed to create descriptor set layout!");
//         }

//         VkDescriptorSetLayoutBinding uboLayoutBinding{};
//         uboLayoutBinding.binding = 0;
//         uboLayoutBinding.descriptorCount = 1;
//         uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//         uboLayoutBinding.pImmutableSamplers = nullptr;
//         uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//         std::array<VkDescriptorSetLayoutBinding, 1> uboBindings = {uboLayoutBinding};
//         VkDescriptorSetLayoutCreateInfo uboLayoutInfo{};
//         uboLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//         uboLayoutInfo.bindingCount = static_cast<uint32_t>(uboBindings.size());
//         uboLayoutInfo.pBindings = uboBindings.data();
//         if (vkCreateDescriptorSetLayout(context->device.logical, &uboLayoutInfo, nullptr, &uboLayout) != VK_SUCCESS) {
//           throw std::runtime_error("failed to create descriptor set layout!");
//         }
//       }

//       virtual std::vector<VkDescriptorSetLayout> descriptorSetLayouts() {
//         return {uboLayout, textureLayout};
//       }

//       virtual void cleanupDescriptorSetLayouts(){
//         vkDestroyDescriptorSetLayout(context->device.logical, textureLayout, nullptr);
//         vkDestroyDescriptorSetLayout(context->device.logical, uboLayout, nullptr);
//       }

//       virtual std::vector<VkPushConstantRange> pushConstantRanges() {
//         VkPushConstantRange pushConstantRange{};
//         pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//         pushConstantRange.offset = 0;
//         pushConstantRange.size = sizeof(glm::mat4);
//         return { pushConstantRange };
//       }

//       void setupVertexInputInfo(VkPipelineVertexInputStateCreateInfo* info) {
//         info->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//         auto bindingDescription = VulkanVertex::getBindingDescription();
//         auto attributeDescriptions = VulkanVertex::getAttributeDescriptions();
//         info->vertexBindingDescriptionCount = 1;
//         info->vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
//         info->pVertexBindingDescriptions = &bindingDescription;
//         info->pVertexAttributeDescriptions = attributeDescriptions.data();        
//       }
//     };

//   }
// }