#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "memory/buffer.h"
#include "render-vulkan/device.h"

namespace wage {
  namespace render {

    class Shader {
      public:
        Shader(Device* device, VkShaderStageFlagBits stage) : device(device), stage(stage) {}

        inline void create(const memory::Buffer& code) {
          VkShaderModuleCreateInfo createInfo{};
          createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
          createInfo.codeSize = code.length();
          createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
          if (vkCreateShaderModule(device->logical(), &createInfo, nullptr, &_module) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
          }
        }

        inline VkPipelineShaderStageCreateInfo createInfo() {
          VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
          vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
          vertShaderStageInfo.stage = stage;
          vertShaderStageInfo.module = _module;
          vertShaderStageInfo.pName = "main";
          return vertShaderStageInfo;
        }

        inline void destroy() {
          vkDestroyShaderModule(device->logical(), _module, nullptr);
        }

        inline VkShaderModule module() {
          return _module;
        }

      private:
        Device* device;
        VkShaderStageFlagBits stage;
        VkShaderModule _module;
    };
  } 
}