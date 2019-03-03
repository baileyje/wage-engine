#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "render/renderer.h"
#include "render/queue.h"

#include "render-gl/texture_manager.h"
#include "render-gl/shader_manager.h"
#include "render-gl/vao_manager.h"
#include "render-gl/font_manager.h"


namespace wage {

  class GlRenderer : public Renderer {
  
  public:

    void start();

    void renderText(Vector position, std::string text, Font font, Color color);

    void renderMesh(Reference<Transform> transform, Reference<Mesh> mesh, Reference<Material> material);
    
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