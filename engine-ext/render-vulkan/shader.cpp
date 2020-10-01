#include "render-vulkan/shader.h"

#include "render-vulkan/device.h"

namespace wage::render {

  Shader::Shader(Device* device, VkShaderStageFlagBits stage) : stage(stage), device(device) {}

  void Shader::create(const memory::Buffer& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.length();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    if (vkCreateShaderModule(device->logical, &createInfo, nullptr, &module) != VK_SUCCESS) {
      throw std::runtime_error("failed to create shader module!");
    }
  }

  VkPipelineShaderStageCreateInfo Shader::createInfo() {
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = stage;
    vertShaderStageInfo.module = module;
    vertShaderStageInfo.pName = "main";
    return vertShaderStageInfo;
  }

  void Shader::destroy() {
    vkDestroyShaderModule(device->logical, module, nullptr);
  }
}