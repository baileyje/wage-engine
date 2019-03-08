#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>

#include "core/service.h"
#include "platform/window.h"
#include "assets/manager.h"
#include "ecs/entity_manager.h"

#include "component/lighting/directional_light.h"
#include "component/lighting/point_light.h"
#include "component/lighting/spotlight.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/font.h"
#include "render/renderable.h"
#include "render/queue.h"

namespace wage {

  class Renderer : public Service {

  public:

    Renderer() : Service("Renderer") { }

    virtual ~Renderer();

    virtual void start();

    virtual void render();

    virtual void renderText(Vector position, std::string text, Font font, Color color) = 0;

    virtual void renderMesh(Reference<Transform> transform, Reference<Mesh, ComponentId> mesh, Reference<Material, ComponentId> material) = 0;
  
  protected:

    void renderMeshes(EntityManager* manager, RenderContext* renderContext);

    void renderUi(EntityManager* manager, RenderContext* renderContext);

    virtual void beginRender() = 0;

    virtual void endRender() = 0;
  
  protected:

    Window* window;

    AssetManager* assetManager;

    RenderQueue meshQueue;

    RenderQueue uiQueue;
  };

}

#endif //RENDERER_H