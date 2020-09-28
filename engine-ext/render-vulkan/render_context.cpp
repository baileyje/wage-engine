#include "render-vulkan/render_context.h"

#include "render/components/camera/camera.h"
#include "render-vulkan/device.h"
#include "render-vulkan/command_pool.h"
#include "render-vulkan/model_pipeline.h"

namespace wage {
  namespace render {
    VulkanRenderContext::VulkanRenderContext(ecs::Entity cameraEntity, Camera* camera, math::Vector2 screenSize, std::vector<ecs::Entity> dirLights, std::vector<ecs::Entity> pointLights, std::vector<ecs::Entity> spotlights) 
      : RenderContext(cameraEntity, camera, screenSize, dirLights, pointLights, spotlights) {
    }
  }
}