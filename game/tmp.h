#pragma once

#include "engine.h"
#include "debug.h"

using namespace wage;

class ColliderDebug : public ecs::System {

public:
  bool enabled = false;

  ColliderDebug() : System() {
  }

  void update(const ecs::SystemContext& context) {
    auto input = core::Core::Instance->get<input::Input>();
    if (input->wasPressed(input::Key::f5)) {
      enabled = !enabled;
    }

    if (!enabled) return;
    auto& entities = scene::Scene::current().entities();
    auto renderer = core::Core::Instance->get<render::Renderer>();
    for (auto entity : entities.with({ColliderComponent, TransformComponent})) {
      if (!entity.valid()) {
        continue;
      }
      auto transform = entity.get<math::Transform>(TransformComponent);
      auto collider = entity.get<physics::Collider>(ColliderComponent);
      // TODO: FIX THIS BEFORE IT GOES ANY FURTHER AND KILLS SOMEONE.
      math::Transform jank(
          collider->transform.position(transform->localPosition),
          collider->transform.scale(transform->localScale),
          collider->transform.rotation(transform->localRotation));
      if (collider->type == physics::ColliderType::box)
        renderer->renderWireframe(jank, {"cube.obj"});
      else if (collider->type == physics::ColliderType::sphere)
        renderer->renderWireframe(jank, {render::MeshSpec::Sphere});
    }
  }
};