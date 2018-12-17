#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "entity/entity.h"

#include "render/renderer.h"
#include "render-gl/texture_manager.h"
#include "render-gl/vao_manager.h"


namespace wage {

  class GlRenderer : public Renderer {
  
  public:

    LIFECYCLE_FUNC(start)

    virtual Renderable* meshRenderable(EntityReference entity);
    
    virtual void beginUpdate();

    virtual void endUpdate();

  private:

    VaoManager vaoManager;

    GlTextureManager textureManager;

    std::vector<DirectionalLight*> dirLights;

    std::vector<PointLight*> pointLights;

    std::vector<Spotlight*> spotlights;
    
  };

}

#endif //GL_RENDERER_H