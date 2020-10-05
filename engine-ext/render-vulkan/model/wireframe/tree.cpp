#include "render-vulkan/model/wireframe/tree.h"

#include "core/logger.h"

#include "render-vulkan/model/mesh.h"
#include "render-vulkan/model/pipeline.h"
#include "render-vulkan/render_context.h"
#include "render-vulkan/scene.h"
#include "render-vulkan/model/mesh_manager.h"

namespace wage::render::vulkan {

  void WireframeTree::add(MeshSpec mesh, math::Matrix modelMatrix) {
    auto meshKey = mesh.key();
    if (groups.find(meshKey) == groups.end()) {
      groups[meshKey].meshSpec = mesh;
    }
    groups[meshKey].modelMatrices.push_back(modelMatrix);
  }

  void WireframeTree::render(VulkanRenderContext* context) {
    core::Logger::debug("Rendering model tree");

    for (const auto& [k1, meshGroup] : groups) {
      auto mesh = context->meshManager->load(meshGroup.meshSpec);
      if (!mesh->loaded()) continue;
      mesh->push(context->device, context->commandPool);
      mesh->bind(context->commandBuffer);
      // Bind mesh
      for (auto& modelMatrix : meshGroup.modelMatrices) {
        auto glmMatrix = modelMatrix.glm();
        vkCmdPushConstants(context->commandBuffer, context->modelPipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &glmMatrix);
        vkCmdDrawIndexed(context->commandBuffer, static_cast<uint32_t>(mesh->meshData->indices.size()), 1, 0, 0, 0);
      }
    }
  }
}