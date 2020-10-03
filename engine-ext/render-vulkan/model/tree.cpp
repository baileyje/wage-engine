#include "render-vulkan/model/tree.h"

#include "core/logger.h"

#include "render-vulkan/core/texture.h"
#include "render-vulkan/model/texture_asset.h"
#include "render-vulkan/model/mesh.h"
#include "render-vulkan/model/pipeline.h"
#include "render-vulkan/render_context.h"
#include "render-vulkan/scene.h"
#include "render-vulkan/model/mesh_manager.h"
#include "render-vulkan/model/texture_manager.h" 

namespace wage::render::vulkan {

  void ModelTree::add(TextureSpec texture, MeshSpec mesh, math::Transform transform) {
    auto textureKey = texture.key();
    auto meshKey = mesh.key();
    if (groups.find(textureKey) == groups.end()) {
      groups[textureKey].textureSpec = texture;
    }
    if (groups[textureKey].meshes.find(meshKey) == groups[textureKey].meshes.end()) {
      groups[textureKey].meshes[meshKey].meshSpec = mesh;
    }
    groups[textureKey].meshes[meshKey].transforms.push_back(transform);
  }

  void ModelTree::render(VulkanRenderContext* context) {
    core::Logger::debug("Rendering model tree");

    // model->draw(context, transform);

    for (const auto & [k1, textureGroup] : groups) {
      auto textureAsset = context->textureManager->load(textureGroup.textureSpec);
      if (!textureAsset->loaded()) continue;
      textureAsset->texture.push(context->device, context->commandPool, context->modelPipeline->textureLayout, context->scene->descriptorPool, context->imageCount);
      textureAsset->texture.bind(context->commandBuffer, context->modelPipeline->layout, context->imageIndex);
      // Bind Texture
      for (const auto & [k2, meshGroup] : textureGroup.meshes) {
        auto mesh = context->meshManager->load(meshGroup.meshSpec);
        if (!mesh->loaded()) continue;
        mesh->push(context->device, context->commandPool);
        mesh->bind(context->commandBuffer);
        // Bind mesh
        for (auto& transform : meshGroup.transforms) {
          auto modelMatrix = transform.worldProjection().glm();
          vkCmdPushConstants(context->commandBuffer, context->modelPipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMatrix);
          vkCmdDrawIndexed(context->commandBuffer, static_cast<uint32_t>(mesh->meshData->indices.size()), 1, 0, 0, 0);
        }
      }
    }
  }
}