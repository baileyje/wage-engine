#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H

namespace wage {

#include "ecs/system.h"

  class MeshRenderer : public System {
  public:
    MeshRenderer() : System("MeshRenderer") {
    }

    void update(SystemContext* context) {
      auto manager = Core::Instance->get<EntityManager>();
      for (auto entity : manager->registry()->with<Mesh>()) {
        auto trans = entity.get<Transform>();
        auto mesh = entity.get<Mesh>();
        auto mat = entity.get<Material>();
        Core::Instance->get<Renderer>()->renderMesh(trans, mesh, mat);
      }
    }
  };
}

#endif // RENDERER_SYSTEM_H