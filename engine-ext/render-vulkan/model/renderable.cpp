#include "render-vulkan/model/renderable.h"
#include "render-vulkan/render_context.h"

namespace wage::render::vulkan {

  ModelRenderable::ModelRenderable(math::Transform transform, MeshSpec meshSpec, MaterialSpec material)
      : transform(transform), meshSpec(meshSpec), textureSpec(material.texture()) {
  }

  math::Vector ModelRenderable::position() {
    return transform.position();
  }

  math::BoundingBox ModelRenderable::boundingBox() {
    // TODO: Lookup mesh data from manager to get dims
    // math::Vector maxDims = model->mesh->meshData->maxDim;
    // math::Vector scale = transform.scale();
    // math::Vector scaledMaxHalfDim(
    //     maxDims.x * scale.x,
    //     maxDims.y * scale.y,
    //     maxDims.z * scale.z);
    // return math::BoundingBox(position(), scaledMaxHalfDim);
    return math::BoundingBox(position(), {1, 1, 1});
  }

  math::BoundingSphere ModelRenderable::boundingSphere() {
    auto box = boundingBox();
    float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);
    return math::BoundingSphere(box.position, radius);
  }

  void ModelRenderable::render(RenderContext* context) {
    auto vkContext = static_cast<VulkanRenderContext*>(context);
    vkContext->modelTree.add(textureSpec, meshSpec, meshSpec.transform.worldProjection(transform));
  }

}