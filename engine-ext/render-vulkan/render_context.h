#pragma once

#include <vector>

#include "render-vulkan/core/common.h"
#include "render-vulkan/model/tree.h"
#include "render-vulkan/model/wireframe/tree.h"
#include "render/context.h"
#include "asset/manager.h"
#include "render/components/camera/camera.h"
#include "ecs/entity.h"
#include "math/vector.h"

namespace wage::render::vulkan {

  class Device;
  class ModelPipeline;
  class WireframePipeline;
  class TextPipeline;
  class CommandPool;
  class VulkanScene;
  class VulkanMeshManager;
  class VulkanTextureManager;

  class VulkanRenderContext : public RenderContext {
  public:

    Device* device;

    VulkanScene* scene;

    ModelPipeline* modelPipeline;

    WireframePipeline* wireframePipeline;

    TextPipeline* textPipeline;

    CommandPool* commandPool;

    uint32_t imageIndex;

    VkCommandBuffer commandBuffer;

    uint8_t imageCount;

    std::vector<std::function<void(VulkanRenderContext*)>> cleanups;

    ModelTree modelTree;

    WireframeTree wireframeTree;

    asset::Manager *assetManager;
    
    VulkanMeshManager *meshManager;

    VulkanTextureManager* textureManager;

    VulkanRenderContext(ecs::Entity cameraEntity, Camera*camera, math::Vector2 screenSize, std::vector<ecs::Entity> dirLights, std::vector<ecs::Entity> pointLights, std::vector<ecs::Entity> spotlights);
  };

}