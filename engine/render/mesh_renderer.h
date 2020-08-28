#pragma once

namespace wage {
  namespace render {

#include "ecs/system.h"

    class MeshRenderer : public ecs::System {
    public:
      MeshRenderer() : System() {
      }

      void update(const ecs::SystemContext& context) {
        auto manager = core::Core::Instance->get<ecs::EntityManager>();
        for (auto entity : manager->with({MeshComponent, TransformComponent, MaterialComponent})) {
          core::Core::Instance->get<Renderer>()->renderMesh(*entity.get<math::Transform>(TransformComponent), entity.get<MeshSpec>(MeshComponent), entity.get<MaterialSpec>(MaterialComponent));
        }
      }
    };

  }
}