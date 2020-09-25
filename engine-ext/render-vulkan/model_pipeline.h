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

      //   void create() {
      //     createRenderPass();
      //     createDescriptorSetLayouts();
      //     auto fs = core::Core::Instance->get<fs::FileSystem>();
      //     Shader vertShader(device, VK_SHADER_STAGE_VERTEX_BIT);
      //     vertShader.create(fs->read({{"resources/shader/vert.spv"}}, memory::Allocator::Temporary()));
      //     Shader fragShader(device, VK_SHADER_STAGE_FRAGMENT_BIT);
      //     fragShader.create(fs->read({{"resources/shader/frag.spv"}}, memory::Allocator::Temporary()));

      //     VkPipelineShaderStageCreateInfo shaderStages[] = {vertShader.createInfo(), fragShader.createInfo()};

      //     VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
      //     vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      //     auto bindingDescription = VulkanVertex::getBindingDescription();
      //     auto attributeDescriptions = VulkanVertex::getAttributeDescriptions();
      //     vertexInputInfo.vertexBindingDescriptionCount = 1;
      //     vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
      //     vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
      //     vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

      //     VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
      //     inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      //     inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
      //     inputAssembly.primitiveRestartEnable = VK_FALSE;

      //     VkViewport viewport{};
      //     viewport.x = 0.0f;
      //     viewport.y = 0.0f;
      //     viewport.width = (float)swapChain->extent.width;
      //     viewport.height = (float)swapChain->extent.height;
      //     viewport.minDepth = 0.0f;
      //     viewport.maxDepth = 1.0f;

      //     VkRect2D scissor{};
      //     scissor.offset = {0, 0};
      //     scissor.extent = swapChain->extent;

      //     VkPipelineViewportStateCreateInfo viewportState{};
      //     viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      //     viewportState.viewportCount = 1;
      //     viewportState.pViewports = &viewport;
      //     viewportState.scissorCount = 1;
      //     viewportState.pScissors = &scissor;

      //     VkPipelineRasterizationStateCreateInfo rasterizer{};
      //     rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      //     rasterizer.depthClampEnable = VK_FALSE;
      //     rasterizer.rasterizerDiscardEnable = VK_FALSE;
      //     rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
      //     rasterizer.lineWidth = 1.0f;
      //     rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
      //     rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
      //     rasterizer.depthBiasEnable = VK_FALSE;

      //     VkPipelineMultisampleStateCreateInfo multisampling{};
      //     multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      //     multisampling.sampleShadingEnable = VK_FALSE;
      //     multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

      //     VkPipelineDepthStencilStateCreateInfo depthStencil{};
      //     depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
      //     depthStencil.depthTestEnable = VK_TRUE;
      //     depthStencil.depthWriteEnable = VK_TRUE;
      //     depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
      //     depthStencil.depthBoundsTestEnable = VK_FALSE;
      //     depthStencil.stencilTestEnable = VK_FALSE;

      //     VkPipelineColorBlendAttachmentState colorBlendAttachment{};
      //     colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
      //     colorBlendAttachment.blendEnable = VK_FALSE;

      //     VkPipelineColorBlendStateCreateInfo colorBlending{};
      //     colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      //     colorBlending.logicOpEnable = VK_FALSE;
      //     colorBlending.logicOp = VK_LOGIC_OP_COPY;
      //     colorBlending.attachmentCount = 1;
      //     colorBlending.pAttachments = &colorBlendAttachment;
      //     colorBlending.blendConstants[0] = 0.0f;
      //     colorBlending.blendConstants[1] = 0.0f;
      //     colorBlending.blendConstants[2] = 0.0f;
      //     colorBlending.blendConstants[3] = 0.0f;

      //     VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      //     pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      //     std::array<VkDescriptorSetLayout, 2> setLayouts = {sceneUboLayout, modelTextureLayout};
      //     pipelineLayoutInfo.setLayoutCount = setLayouts.size();
      //     pipelineLayoutInfo.pSetLayouts = setLayouts.data();

      //     // We will use push constants to push the local matrices of a primitive to the vertex shader
      //     VkPushConstantRange pushConstantRange{};
      //     pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
      //     pushConstantRange.offset = 0;
      //     pushConstantRange.size = sizeof(glm::mat4);
      //     // Push constant ranges are part of the pipeline layout
      //     pipelineLayoutInfo.pushConstantRangeCount = 1;
      //     pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

      //     if (vkCreatePipelineLayout(device->logical, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      //       throw std::runtime_error("failed to create pipeline layout!");
      //     }

      //     VkGraphicsPipelineCreateInfo pipelineInfo{};
      //     pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      //     pipelineInfo.stageCount = 2;
      //     pipelineInfo.pStages = shaderStages;
      //     pipelineInfo.pVertexInputState = &vertexInputInfo;
      //     pipelineInfo.pInputAssemblyState = &inputAssembly;
      //     pipelineInfo.pViewportState = &viewportState;
      //     pipelineInfo.pRasterizationState = &rasterizer;
      //     pipelineInfo.pMultisampleState = &multisampling;
      //     pipelineInfo.pDepthStencilState = &depthStencil;
      //     pipelineInfo.pColorBlendState = &colorBlending;
      //     pipelineInfo.layout = pipelineLayout;
      //     pipelineInfo.renderPass = _renderPass;
      //     pipelineInfo.subpass = 0;
      //     pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

      //     if (vkCreateGraphicsPipelines(device->logical, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_wrapped) != VK_SUCCESS) {
      //       throw std::runtime_error("failed to create graphics pipeline!");
      //     }
      //     fragShader.destroy();
      //     vertShader.destroy();
      //   }

      //   void cleanup() {
      //     vkDestroyPipeline(device->logical, _wrapped, nullptr);
      //     vkDestroyPipelineLayout(device->logical, pipelineLayout, nullptr);
      //     vkDestroyRenderPass(device->logical, _renderPass, nullptr);
      //     vkDestroyDescriptorSetLayout(device->logical, modelTextureLayout, nullptr);
      //   }

      //   inline VkPipeline wrapped() {
      //     return _wrapped;
      //   }

      //   inline VkRenderPass renderPass() {
      //     return _renderPass;
      //   }

      //   inline VkPipelineLayout layout() {
      //     return pipelineLayout;
      //   }

 

      // private:
      //   VkShaderModule createShaderModule(const memory::Buffer &code) {
      //     VkShaderModuleCreateInfo createInfo{};
      //     createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      //     createInfo.codeSize = code.length();
      //     createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

      //     VkShaderModule shaderModule;
      //     if (vkCreateShaderModule(device->logical, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
      //       throw std::runtime_error("failed to create shader module!");
      //     }
      //     return shaderModule;
      //   }

      //   void createRenderPass() {
      //     VkAttachmentDescription colorAttachment{};
      //     colorAttachment.format = swapChain->imageFormat;
      //     colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
      //     colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      //     colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      //     colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      //     colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      //     colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      //     colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      //     VkAttachmentDescription depthAttachment{};
      //     depthAttachment.format = findDepthFormat(device->physical);
      //     depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
      //     depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      //     depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      //     depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      //     depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      //     depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      //     depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

      //     VkAttachmentReference colorAttachmentRef{};
      //     colorAttachmentRef.attachment = 0;
      //     colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      //     VkAttachmentReference depthAttachmentRef{};
      //     depthAttachmentRef.attachment = 1;
      //     depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

      //     VkSubpassDescription subpass{};
      //     subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      //     subpass.colorAttachmentCount = 1;
      //     subpass.pColorAttachments = &colorAttachmentRef;
      //     subpass.pDepthStencilAttachment = &depthAttachmentRef;

      //     VkSubpassDependency dependency{};
      //     dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
      //     dependency.dstSubpass = 0;
      //     dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      //     dependency.srcAccessMask = 0;
      //     dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      //     dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

      //     std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
      //     VkRenderPassCreateInfo renderPassInfo{};
      //     renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      //     renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
      //     renderPassInfo.pAttachments = attachments.data();
      //     renderPassInfo.subpassCount = 1;
      //     renderPassInfo.pSubpasses = &subpass;
      //     renderPassInfo.dependencyCount = 1;
      //     renderPassInfo.pDependencies = &dependency;

      //     if (vkCreateRenderPass(device->logical, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) {
      //         throw std::runtime_error("failed to create render pass!");
      //     }
      //   }

      //   void createDescriptorSetLayouts() {
      //     VkDescriptorSetLayoutBinding samplerLayoutBinding{};
      //     samplerLayoutBinding.binding = 0;
      //     samplerLayoutBinding.descriptorCount = 1;
      //     samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      //     samplerLayoutBinding.pImmutableSamplers = nullptr;
      //     samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
      //     std::array<VkDescriptorSetLayoutBinding,1> samplerBindings = {samplerLayoutBinding};
      //     VkDescriptorSetLayoutCreateInfo samplerLayoutInfo{};
      //     samplerLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      //     samplerLayoutInfo.bindingCount = static_cast<uint32_t>(samplerBindings.size());
      //     samplerLayoutInfo.pBindings = samplerBindings.data();
      //     if (vkCreateDescriptorSetLayout(device->logical, &samplerLayoutInfo, nullptr, &modelTextureLayout) != VK_SUCCESS) {
      //       throw std::runtime_error("failed to create descriptor set layout!");
      //     }

      //     VkDescriptorSetLayoutBinding uboLayoutBinding{};
      //     uboLayoutBinding.binding = 0;
      //     uboLayoutBinding.descriptorCount = 1;
      //     uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      //     uboLayoutBinding.pImmutableSamplers = nullptr;
      //     uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
      //     std::array<VkDescriptorSetLayoutBinding, 1> uboBindings = {uboLayoutBinding};
      //     VkDescriptorSetLayoutCreateInfo uboLayoutInfo{};
      //     uboLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      //     uboLayoutInfo.bindingCount = static_cast<uint32_t>(uboBindings.size());
      //     uboLayoutInfo.pBindings = uboBindings.data();
      //     if (vkCreateDescriptorSetLayout(device->logical, &uboLayoutInfo, nullptr, &sceneUboLayout) != VK_SUCCESS) {
      //       throw std::runtime_error("failed to create descriptor set layout!");
      //     }
      //   }

      //   Device* device;
      //   SwapChain* swapChain;

      //   VkPipeline _wrapped;
      //   VkRenderPass _renderPass;
      //   VkPipelineLayout pipelineLayout;
      
    };

  }
}