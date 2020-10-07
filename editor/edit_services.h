#pragma once

#include "engine.h"

namespace wage::editor {

  class EditorController : public core::Service, public messaging::MessageListener<input::KeyEvent> {
  public:
    EditorController() : Service("EditorController") {}

    void start() {
      core::Core::Instance->get<messaging::Messaging>()->listen<input::KeyEvent>(this);
    }

    inline bool on(const input::KeyEvent& event) {

      std::cout << "Key\n";

      return false;
    }

  private:
  };

  class EditorGizmos : public core::Service {
  public:
    EditorGizmos() : Service("EditorGizmos") {}

    void start() {
      core::Core::Instance->onUpdate([&](const core::Frame& frame) {
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
          if (collider->type == physics::ColliderType::box)
            renderer->renderWireframe(colliderRenderTransform, {"cube.obj"});
          else if (collider->type == physics::ColliderType::sphere)
            renderer->renderWireframe(colliderRenderTransform, {render::MeshSpec::Sphere});
        }
        for (auto entity : entities.with({TransformComponent})) {
          auto transform = entity.get<math::Transform>(TransformComponent);
          auto scale = transform->scale();
          math::Transform forwardHandleTransform(math::Transform({0, 0, 2 / scale.z}, {.1f / scale.x, .1f / scale.y, 4.0f / scale.z}, math::Quaternion()).worldProjection(*transform));
          renderer->renderMesh(forwardHandleTransform, {"cube.obj"}, {{"blue.png"}});

          math::Transform upHandleTransform(math::Transform({0, 2 / scale.y, 0}, {.1f / scale.x, 4.0f / scale.y, .1f / scale.z}, math::Quaternion()).worldProjection(*transform));
          renderer->renderMesh(upHandleTransform, {"cube.obj"}, {{"green.png"}});

          math::Transform rightHandleTransform(math::Transform({-2 / scale.x, 0, 0}, {4.0f / scale.x, .1f / scale.y, .1f / scale.z}, math::Quaternion()).worldProjection(*transform));
          renderer->renderMesh(rightHandleTransform, {"cube.obj"}, {{"red.png"}});
        }
      });
    }

  private:
  };
}
