#pragma once

#include "asset/manager.h"
#include "render/renderable.h"
#include "render/context.h"

#include "render/components/mesh.h"
#include "render/components/material.h"


namespace wage::render::vulkan {

  class ModelRenderable : public Renderable {

  public:
    ModelRenderable(math::Transform transform, MeshSpec meshSpec, MaterialSpec material);

    virtual math::Vector position();

    virtual math::BoundingBox boundingBox();

    virtual math::BoundingSphere boundingSphere();

    virtual void render(RenderContext* context);

  private:
    math::Transform transform;

    MeshSpec meshSpec;

    TextureSpec textureSpec;

  };
}