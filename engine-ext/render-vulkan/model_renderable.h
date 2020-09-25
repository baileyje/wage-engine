#pragma once

#include "asset/manager.h"
#include "render/renderable.h"
#include "render/context.h"

#include "render/components/mesh.h"
#include "render/components/material.h"


namespace wage {
  namespace render {

    class MeshManager;
    class ModelManager;
    class Model;

    class ModelRenderable : public Renderable {

    public:
      ModelRenderable(asset::Manager* assetManager, MeshManager* meshManager, ModelManager* modelManager, math::Transform transform, MeshSpec* meshSpec, MaterialSpec* material);

      virtual math::Vector position();

      virtual math::BoundingBox boundingBox();

      virtual math::BoundingSphere boundingSphere();

      virtual void render(RenderContext * context);

    private:

      math::Transform transform;

      MeshSpec meshSpec;
      
      TextureSpec textureSpec;

      Model* model;
    };
  }
}