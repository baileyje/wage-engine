#pragma once

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
#include "render-vulkan/core/pipeline.h"

namespace wage::render::vulkan {

  class TextPipeline : public Pipeline {
  public:
  
    VkDescriptorSetLayout textureLayout;

    TextPipeline(VulkanContext* context);

    void create();

    void cleanup();

    void createDescriptorSetLayouts();
  };

}