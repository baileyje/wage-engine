#pragma once

namespace wage { namespace render {

#include "ecs/system.h"

  class MeshRenderer : public ecs::System {
  public:
    MeshRenderer() : System() {
    }

    void update(const ecs::SystemContext& context) {
      auto manager = core::Core::Instance->get<ecs::EntityManager>();
      for (auto entity : manager->with<Mesh>()) {
        auto trans = entity.get<math::Transform>();
        auto mesh = entity.get<Mesh>();
        auto mat = entity.get<Material>();
        core::Core::Instance->get<Renderer>()->renderMesh(*trans, mesh.get(), mat.get());
      }
    }
  };

} }