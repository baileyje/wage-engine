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