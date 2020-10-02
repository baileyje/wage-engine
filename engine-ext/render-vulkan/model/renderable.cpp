#include "render-vulkan/model/renderable.h"

#include "render-vulkan/render_context.h"
#include "render-vulkan/model/model.h"
#include "render-vulkan/model/manager.h"
#include "render-vulkan/core/pipeline.h"

namespace wage::render::vulkan {

  ModelRenderable::ModelRenderable(asset::Manager* assetManager, render::MeshManager* meshManager, ModelManager* modelManager, math::Transform transform, MeshSpec* meshSpec, MaterialSpec* material)
      : transform(transform), meshSpec(*meshSpec), textureSpec(material->texture()) {
    model = modelManager->load(assetManager, meshManager, *meshSpec, textureSpec);
  }

  math::Vector ModelRenderable::position() {
    return transform.position();
  }

  math::BoundingBox ModelRenderable::boundingBox() {
    math::Vector maxDims = model->mesh->meshData->maxDim;
    math::Vector scale = transform.scale();
    math::Vector scaledMaxHalfDim(
        maxDims.x * scale.x,
        maxDims.y * scale.y,
        maxDims.z * scale.z);
    return math::BoundingBox(position(), scaledMaxHalfDim);
    return math::BoundingBox(position(), {1, 1, 1});
  }

  math::BoundingSphere ModelRenderable::boundingSphere() {
    auto box = boundingBox();
    float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);
    return math::BoundingSphere(box.position, radius);
  }

  void ModelRenderable::render(RenderContext* context) {
    auto vkContext = static_cast<VulkanRenderContext*>(context);
    if (!model->loaded()) {
      return;
    }
    model->push(vkContext->device, vkContext->commandPool, vkContext->modelPipeline, vkContext->imageCount);
    // std::cout << "Ctx: " << vkContext << "\n";
    // START: MODEL
    VkBuffer vertexBuffers[] = {model->mesh->vertexBuffer.buffer};
    VkDeviceSize offsets[] = {0};
    // Bind per mesh bucket
    vkCmdBindVertexBuffers(vkContext->commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(vkContext->commandBuffer, model->mesh->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    // Apply transform to mesh
    auto modelMat = transform.worldProjection().glm();
    vkCmdPushConstants(vkContext->commandBuffer, vkContext->modelPipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMat);
    vkCmdBindDescriptorSets(vkContext->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkContext->modelPipeline->layout, 1, 1, &model->textureAsset->texture.descriptorSets[vkContext->imageIndex], 0, nullptr);
    // Draw.
    vkCmdDrawIndexed(vkContext->commandBuffer, static_cast<uint32_t>(model->mesh->meshData->indices.size()), 1, 0, 0, 0);
    // END: MODEL
  }

}