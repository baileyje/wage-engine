#include "render-vulkan/core/pipeline.h"

#include <array>

#include "core/core.h"
#include "fs/file_system.h"
#include "memory/allocator.h"
#include "memory/buffer.h"

#include "render-vulkan/core/device.h"
#include "render-vulkan/core/swap_chain.h"
#include "render-vulkan/core/shader.h"
#include "render-vulkan/core/vertex.h"
#include "render-vulkan/core/render_pass.h"
#include "render-vulkan/core/context.h"

namespace wage::render::vulkan {

  Pipeline::Pipeline(VulkanContext* context) : context(context) {}

  // void Pipeline::create() {
  //   createDescriptorSetLayouts();
  //   VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  //   vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  //   auto bindingDescription = VulkanVertex::getBindingDescription();
  //   auto attributeDescriptions = VulkanVertex::getAttributeDescriptions();
  //   vertexInputInfo.vertexBindingDescriptionCount = 1;
  //   vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
  //   vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  //   vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  //   VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  //   inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  //   inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  //   inputAssembly.primitiveRestartEnable = VK_FALSE;

  //   VkViewport viewport{};
  //   viewport.x = 0.0f;
  //   viewport.y = 0.0f;
  //   viewport.width = (float)context->swapChain.extent.width;
  //   viewport.height = (float)context->swapChain.extent.height;
  //   viewport.minDepth = 0.0f;
  //   viewport.maxDepth = 1.0f;

  //   VkRect2D scissor{};
  //   scissor.offset = {0, 0};
  //   scissor.extent = context->swapChain.extent;

  //   VkPipelineViewportStateCreateInfo viewportState{};
  //   viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  //   viewportState.viewportCount = 1;
  //   viewportState.pViewports = &viewport;
  //   viewportState.scissorCount = 1;
  //   viewportState.pScissors = &scissor;

  //   VkPipelineRasterizationStateCreateInfo rasterizer{};
  //   rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  //   rasterizer.depthClampEnable = VK_FALSE;
  //   rasterizer.rasterizerDiscardEnable = VK_FALSE;
  //   rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  //   rasterizer.lineWidth = 1.0f;
  //   rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  //   rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  //   rasterizer.depthBiasEnable = VK_FALSE;

  //   VkPipelineMultisampleStateCreateInfo multisampling{};
  //   multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  //   multisampling.sampleShadingEnable = VK_FALSE;
  //   multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  //   VkPipelineDepthStencilStateCreateInfo depthStencil{};
  //   depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  //   depthStencil.depthTestEnable = VK_TRUE;
  //   depthStencil.depthWriteEnable = VK_TRUE;
  //   depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
  //   depthStencil.depthBoundsTestEnable = VK_FALSE;
  //   depthStencil.stencilTestEnable = VK_FALSE;

  //   VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  //   colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  //   colorBlendAttachment.blendEnable = VK_TRUE;
  //   colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  //   colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  //   colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  //   colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  //   colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  //   colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  //   VkPipelineColorBlendStateCreateInfo colorBlending{};
  //   colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  //   colorBlending.logicOpEnable = VK_FALSE;
  //   colorBlending.logicOp = VK_LOGIC_OP_COPY;
  //   colorBlending.attachmentCount = 1;
  //   colorBlending.pAttachments = &colorBlendAttachment;
  //   colorBlending.blendConstants[0] = 0.0f;
  //   colorBlending.blendConstants[1] = 0.0f;
  //   colorBlending.blendConstants[2] = 0.0f;
  //   colorBlending.blendConstants[3] = 0.0f;

  //   VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  //   pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  //   auto setLayouts = descriptorSetLayouts();
  //   pipelineLayoutInfo.setLayoutCount = setLayouts.size();
  //   pipelineLayoutInfo.pSetLayouts = setLayouts.data();

  //   auto pushRanges = pushConstantRanges();
  //   pipelineLayoutInfo.pushConstantRangeCount = pushRanges.size();
  //   pipelineLayoutInfo.pPushConstantRanges = pushRanges.data();

  //   if (vkCreatePipelineLayout(context->device.logical, &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
  //     throw std::runtime_error("failed to create pipeline layout!");
  //   }

  //   // VkPipelineShaderStageCreateInfo shaderStages[] = {vertShader.createInfo(), fragShader.createInfo()};

  //   VkGraphicsPipelineCreateInfo pipelineInfo{};
  //   pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

  //   pipelineInfo.pVertexInputState = &vertexInputInfo;
  //   pipelineInfo.pInputAssemblyState = &inputAssembly;
  //   pipelineInfo.pViewportState = &viewportState;
  //   pipelineInfo.pRasterizationState = &rasterizer;
  //   pipelineInfo.pMultisampleState = &multisampling;
  //   pipelineInfo.pDepthStencilState = &depthStencil;
  //   pipelineInfo.pColorBlendState = &colorBlending;
  //   pipelineInfo.layout = layout;
  //   pipelineInfo.renderPass = context->renderPass.wrapped;
  //   pipelineInfo.subpass = 0;
  //   pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  //   std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;
  //   pipelineInfo.stageCount = shaderStages.size();
  //   pipelineInfo.pStages = shaderStages.data();

  //   auto fs = core::Core::Instance->get<fs::FileSystem>();
  //   Shader modelVertShader(&context->device, VK_SHADER_STAGE_VERTEX_BIT);
  //   modelVertShader.create(fs->read({{"resources/shader/model.vert.spv"}}, memory::Allocator::Temporary()));
  //   Shader modelFragShader(&context->device, VK_SHADER_STAGE_FRAGMENT_BIT);
  //   modelFragShader.create(fs->read({{"resources/shader/model.frag.spv"}}, memory::Allocator::Temporary()));
  //   shaderStages[0] = modelVertShader.createInfo();
  //   shaderStages[1] = modelFragShader.createInfo();

  //   if (vkCreateGraphicsPipelines(context->device.logical, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &model) != VK_SUCCESS) {
  //     throw std::runtime_error("failed to create graphics pipeline!");
  //   }
  //   modelVertShader.destroy();
  //   modelFragShader.destroy();

  //   Shader uiVertShader(&context->device, VK_SHADER_STAGE_VERTEX_BIT);
  //   uiVertShader.create(fs->read({{"resources/shader/font.vert.spv"}}, memory::Allocator::Temporary()));
  //   Shader uiFragShader(&context->device, VK_SHADER_STAGE_FRAGMENT_BIT);
  //   uiFragShader.create(fs->read({{"resources/shader/font.frag.spv"}}, memory::Allocator::Temporary()));
  //   shaderStages[0] = uiVertShader.createInfo();
  //   shaderStages[1] = uiFragShader.createInfo();

  //   if (vkCreateGraphicsPipelines(context->device.logical, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ui) != VK_SUCCESS) {
  //     throw std::runtime_error("failed to create graphics pipeline!");
  //   }
  //   uiVertShader.destroy();
  //   uiFragShader.destroy();
  // }

  void Pipeline::cleanup() {
    vkDestroyPipeline(context->device.logical, wrapped, nullptr);
    vkDestroyPipelineLayout(context->device.logical, layout, nullptr);
  }

  void Pipeline::bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, wrapped);
  }

  VkShaderModule Pipeline::createShaderModule(const memory::Buffer& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.length();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(context->device.logical, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
      throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
  }
}