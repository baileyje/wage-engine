#pragma once

#include "ecs/system.h"
#include "scene/scene.h"

namespace wage {
  namespace render {

    /**
     * System that finds any entities that have a mesh component and automatically submits them to the renderer. This system is dumb
     * and grabs every mesh in the entire game to present the renderer. This will eventually be replaced with some time of region
     * based approach to reduce render queue size, culling and sorting.
     */
    class MeshRenderer : public ecs::System {
    public:
      MeshRenderer() : System() {}

      /**
       * On each game update, submit all renderable mesh components to the renderer.
       */
      void update(const ecs::SystemContext& context) {
        auto& entities = scene::Scene::current().entities();
        for (auto entity : entities.with({MeshComponent, TransformComponent, MaterialComponent})) {
          core::Core::Instance->get<Renderer>()->renderMesh(*entity.get<math::Transform>(TransformComponent), *entity.get<MeshSpec>(MeshComponent), *entity.get<MaterialSpec>(MaterialComponent));
        }
      }
    };

  }
}