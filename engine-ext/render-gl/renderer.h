#pragma once

#include "render/renderer.h"
#include "render/queue.h"
#include "render/mesh/manager.h"

#include "render-gl/texture.h"
#include "render-gl/vao_manager.h"
#include "render-gl/program.h"

namespace wage {
  namespace render {

    class GlRenderer : public Renderer {

    public:
      void start();

      void reset() {
        Renderer::reset();
        vaoManager.reset();
        GlProgram::Default->load(assetManager);
        GlProgram::Font->load(assetManager);
        GlProgram::Sprite->load(assetManager);
      }

      void initializeRenderer();

      void renderText(math::Vector2 position, std::string text, FontSpec font, component::Color color);

      void renderSprite(math::Vector2 position, math::Vector2 size, component::Color color, TextureSpec texture);

      void renderMesh(math::Transform transform, MeshSpec mesh, MaterialSpec material);

      virtual void beginRender();

      virtual void endRender();

    private:
      MeshManager meshManager;

      VaoManager vaoManager;

      std::vector<DirectionalLight*> dirLights;

      std::vector<PointLight*> pointLights;

      std::vector<Spotlight*> spotlights;
    };

  }
}