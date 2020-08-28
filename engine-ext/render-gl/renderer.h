#pragma once

#include "render/renderer.h"
#include "render/queue.h"
#include "render/mesh_manager.h"

#include "render-gl/texture.h"
#include "render-gl/vao_manager.h"

namespace wage {
  namespace render {

    class GlRenderer : public Renderer {

    public:
      void start();

      void initializeRenderer();

      void renderText(math::Vector2 position, std::string text, FontSpec font, component::Color color);

      void renderSprite(math::Vector2 position, math::Vector2 size, component::Color color, TextureSpec texture);

      void renderMesh(math::Transform transform, MeshSpec* mesh, MaterialSpec* material);

      virtual void beginRender();

      virtual void endRender();

    private:
      MeshManager meshManager;

      VaoManager vaoManager;

      std::vector<component::DirectionalLight*> dirLights;

      std::vector<component::PointLight*> pointLights;

      std::vector<component::Spotlight*> spotlights;
    };

  }
}