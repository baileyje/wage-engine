#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>

#include "ecs/system.h"
#include "platform/window.h"
#include "assets/manager.h"
#include "entity/manager.h"
#include "render/renderable.h"
#include "render/queue.h"

#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/font.h"

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

    virtual void renderMesh(Reference<Transform> transform, Reference<Mesh, ComponentId> mesh, Reference<Material, ComponentId> material) = 0;
    
  protected:

    virtual void beginUpdate() = 0;

    virtual void endUpdate() = 0;

    virtual void renderMeshes(EntityManager* manager, RenderContext* renderContext);
    
    virtual void renderUi(EntityManager* manager, RenderContext* renderContext);

    Window* window;

    AssetManager* assetManager;

    RenderQueue meshQueue;

    RenderQueue uiQueue;
  };

}

#endif //RENDERER_H