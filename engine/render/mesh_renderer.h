#pragma once

namespace wage {
  namespace render {

#include "ecs/system.h"

    /**
     * System that finds any entities that have a mesh component and automatically submits them to the renderer.
     */
    class MeshRenderer : public ecs::System {
    public:
      MeshRenderer() : System() {}

      /**
       * On each game update, resubmit all renderable mesh components to the renderer.
       */
      void update(const ecs::SystemContext& context) {
        auto manager = core::Core::Instance->get<ecs::EntityManager>();
        for (auto entity : manager->with({MeshComponent, TransformComponent, MaterialComponent})) {
          core::Core::Instance->get<Renderer>()->renderMesh(*entity.get<math::Transform>(TransformComponent), entity.get<MeshSpec>(MeshComponent), entity.get<MaterialSpec>(MaterialComponent));
        }
      }
    };

  }
}