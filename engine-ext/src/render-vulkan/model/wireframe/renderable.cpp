#include "render-vulkan/model/wireframe/renderable.h"
#include "render-vulkan/render_context.h"

namespace wage::render::vulkan {

  WireframeRenderable::WireframeRenderable( math::Transform transform, MeshSpec meshSpec)
      : transform(transform), meshSpec(meshSpec) {
  }

  math::Vector WireframeRenderable::position() {
    return transform.localPosition;
  }

  math::BoundingBox WireframeRenderable::boundingBox() {
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

  math::BoundingSphere WireframeRenderable::boundingSphere() {
    auto box = boundingBox();
    float radius = sqrt(box.halfDim.x * box.halfDim.x + box.halfDim.y * box.halfDim.y + box.halfDim.z * box.halfDim.z);
    return math::BoundingSphere(box.position, radius);
  }

  void WireframeRenderable::render(RenderContext* context) {
    auto vkContext = static_cast<VulkanRenderContext*>(context);
    vkContext->wireframeTree.add(meshSpec, meshSpec.transform.worldProjection(transform));
  }

}