#pragma once

#include <vector>

#include "render-vulkan/common.h"
#include "render/context.h"
#include "render/components/camera/camera.h"
#include "ecs/entity.h"
#include "math/vector.h"
#include "render-vulkan/device.h"
#include "render-vulkan/command_pool.h"
#include "render-vulkan/graphics_pipeline.h"

namespace wage {
  namespace render {

    class VulkanRenderContext : public RenderContext {
      public:
        VulkanRenderContext(ecs::Entity cameraEntity, Camera* camera, math::Vector2 screenSize, std::vector<ecs::Entity> dirLights, std::vector<ecs::Entity> pointLights, std::vector<ecs::Entity> spotlights) : RenderContext(cameraEntity, camera, screenSize, dirLights, pointLights, spotlights) {
        }
        
        Device* device;
        GraphicsPipeline* pipeline;
        CommandPool* commandPool;
        
        uint32_t imageIndex;
        VkCommandBuffer commandBuffer;
        uint8_t imageCount;
    };

  }
}