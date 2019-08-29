#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H

namespace wage {

#include "new_ecs/system.h"

  class MeshRenderer : public System {
  public:
    MeshRenderer() : System() {
    }

    void update(const SystemContext& context) {
      auto manager = Core::Instance->get<EntityManager>();
      for (auto entity : manager->with<Mesh>()) {
        auto trans = entity.get<Transform>();
        auto mesh = entity.get<Mesh>();
        auto mat = entity.get<Material>();
        Core::Instance->get<Renderer>()->renderMesh(trans.get(), mesh.get(), mat.get());
      }
    }
  };
}

#endif // RENDERER_SYSTEM_H