#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "memory/buffer.h"

namespace wage::render {

  class Device;

  class Shader {
  public:
    VkShaderStageFlagBits stage;

    VkShaderModule module;

    Shader(Device* device, VkShaderStageFlagBits stage);

    void create(const memory::Buffer& code);

    VkPipelineShaderStageCreateInfo createInfo();

    void destroy();

  private:
    Device* device;
  };
}