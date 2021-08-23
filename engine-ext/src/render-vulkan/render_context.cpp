#include "render-vulkan/render_context.h"

#include "render/components/camera/camera.h"
#include "render-vulkan/core/device.h"
#include "render-vulkan/core/command_pool.h"
#include "render-vulkan/core/pipeline.h"
#include "render-vulkan/scene.h"
#include "render-vulkan/model/mesh_manager.h"
#include "render-vulkan/model/texture_manager.h"
#include "render-vulkan/model/wireframe/pipeline.h"

namespace wage::render::vulkan {

  VulkanRenderContext::VulkanRenderContext(ecs::Entity cameraEntity, Camera* camera, math::Vector2 screenSize, std::vector<ecs::Entity> dirLights, std::vector<ecs::Entity> pointLights, std::vector<ecs::Entity> spotlights)
      : RenderContext(cameraEntity, camera, screenSize, dirLights, pointLights, spotlights) {
  }
}