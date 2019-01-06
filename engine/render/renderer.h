#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>

#include "engine/core/system.h"
#include "engine/platform/window.h"
#include "engine/assets/manager.h"
#include "engine/entity/manager.h"
#include "engine/render/renderable.h"
#include "engine/render/queue.h"

#include "engine/entity/component/lighting/directional_light.h"
#include "engine/entity/component/lighting/point_light.h"
#include "engine/entity/component/lighting/spotlight.h"
#include "engine/render/material.h"
#include "engine/render/mesh.h"
#include "engine/render/font.h"

namespace wage {

  class Renderer : public System {

  public:

    Renderer() : System("renderer") { }

    virtual ~Renderer();

    LIFECYCLE_FUNC(init)
    
    LIFECYCLE_FUNC(start)
    
    LIFECYCLE_FUNC(update)
    
    LIFECYCLE_FUNC(stop)
    
    LIFECYCLE_FUNC(deinit)

    virtual void renderText(Vector position, std::string text, Font font, Color color) = 0;

    virtual void renderMesh(Transform transform, ComponentReference<Mesh> mesh, ComponentReference<Material> material) = 0;
    
  protected:

    virtual void beginUpdate() = 0;

    virtual void endUpdate() = 0;

    virtual void renderMeshes(EntityManager* manager, RenderContext* renderContext);
    
    virtual void renderUi(EntityManager* manager, RenderContext* renderContext);

    Window* window;

    AssetManager* assetManager;

    // TODO: Hate these here!
    std::vector<DirectionalLight*> dirLights;

    std::vector<PointLight*> pointLights;

    std::vector<Spotlight*> spotlights;

    RenderQueue meshQueue;

    RenderQueue uiQueue;
  };

}

#endif //RENDERER_H