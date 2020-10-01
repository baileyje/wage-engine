#pragma once

#include <vector>

#include "render-vulkan/common.h"
#include "render/context.h"
#include "ecs/entity.h"
#include "math/vector.h"

namespace wage::render {

  class Device;
  class Pipeline;
  class CommandPool;
  class Camera;

  class VulkanRenderContext : public RenderContext {
  public:
    Device* device;

    Pipeline* modelPipeline;

    Pipeline* uiPipeline;

    CommandPool* commandPool;

    uint32_t imageIndex;

    VkCommandBuffer commandBuffer;

    uint8_t imageCount;

    std::vector<std::function<void(VulkanRenderContext*)>> cleanups;

    VulkanRenderContext(ecs::Entity cameraEntity, Camera* camera, math::Vector2 screenSize, std::vector<ecs::Entity> dirLights, std::vector<ecs::Entity> pointLights, std::vector<ecs::Entity> spotlights);
  };

}