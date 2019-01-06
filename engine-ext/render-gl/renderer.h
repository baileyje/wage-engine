#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "engine/entity/entity.h"
#include "engine/render/renderer.h"
#include "engine/render/queue.h"

#include "engine-ext/render-gl/texture_manager.h"
#include "engine-ext/render-gl/shader_manager.h"
#include "engine-ext/render-gl/vao_manager.h"
#include "engine-ext/render-gl/font_manager.h"


namespace wage {

  class GlRenderer : public Renderer {
  
  public:

    LIFECYCLE_FUNC(start)

    void renderText(Vector position, std::string text, Font font, Color color);

    void renderMesh(Transform transform, ComponentReference<Mesh> mesh, ComponentReference<Material> material);
    
    virtual void beginUpdate();

    virtual void endUpdate();

  private:

    VaoManager vaoManager;

    GlTextureManager textureManager;

    GlShaderManager shaderManager;

    GlFontManager fontManager;

    std::vector<DirectionalLight*> dirLights;

    std::vector<PointLight*> pointLights;

    std::vector<Spotlight*> spotlights;    
  };

}

#endif //GL_RENDERER_H