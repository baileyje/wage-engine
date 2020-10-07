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
      math::Transform colliderRenderTransform(collider->transform.worldProjection(*transform));
      auto scale = transform->scale();
      if (collider->type == physics::ColliderType::box)
        renderer->renderWireframe(colliderRenderTransform, {"cube.obj"});
      else if (collider->type == physics::ColliderType::sphere)
        renderer->renderWireframe(colliderRenderTransform, {render::MeshSpec::Sphere});

      Transform forwardHandleTransform(Transform(Vector3(0, 0, 2 / scale.z), {.1f / scale.x, .1f / scale.y, 4.0f / scale.z}, Quaternion()).worldProjection(*transform));
      renderer->renderMesh(forwardHandleTransform, {"cube.obj"}, {{"blue.png"}});

      Transform upHandleTransform(Transform(Vector3(0, 2 / scale.y, 0), {.1f / scale.x, 4.0f / scale.y, .1f / scale.z}, Quaternion()).worldProjection(*transform));
      renderer->renderMesh(upHandleTransform, {"cube.obj"}, {{"green.png"}});

      Transform rightHandleTransform(Transform(Vector3(2 / scale.x, 0, 0), {4.0f / scale.x, .1f / scale.y, .1f / scale.z}, Quaternion()).worldProjection(*transform));
      renderer->renderMesh(rightHandleTransform, {"cube.obj"}, {{"red.png"}});
    }
  }
};