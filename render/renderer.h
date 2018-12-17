#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>

#include "core/system.h"
#include "platform/window.h"
#include "entity/manager.h"
#include "render/renderable.h"

#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"

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
    
  protected:

    Window* window;

    FileSystem* fileSystem;

    virtual void beginUpdate() = 0;

    virtual void endUpdate() = 0;

    virtual Renderable* meshRenderable(EntityReference entity) = 0;

    std::vector<DirectionalLight*> dirLights;

    std::vector<PointLight*> pointLights;

    std::vector<Spotlight*> spotlights;

    private:
    
      void renderMeshes(EntityManager* manager, RenderContext* renderContext);
  };

}

#endif //RENDERER_H