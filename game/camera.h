#pragma once

#include "engine.h"

using namespace wage;

// This is a fairly lame third person rigging
class ThirdPersonCamera : public ecs::System {

public:
  ThirdPersonCamera(ecs::Entity camera) : System(), camera(camera) {
  }

  void start(const ecs::SystemContext& context) {
    auto& entities = scene::Scene::current().entities();
    auto player = *entities.with({PlayerComponent}).begin();
    auto cameraPosition = camera.get<math::Transform>(TransformComponent)->position();
    auto playerPosition = player.get<math::Transform>(TransformComponent)->position();
    followDistance = cameraPosition.distanceTo(playerPosition);
    staringOffset = cameraPosition - playerPosition;
    staringRotation = camera.get<math::Transform>(TransformComponent)->rotation();
  }

  void update(const ecs::SystemContext& context) {
    // Adjust camera position base on player
    auto& entities = scene::Scene::current().entities();
    auto player = *entities.with({PlayerComponent}).begin();
    if (!player.valid())
      return;
    auto bearing = player.get<math::Transform>(TransformComponent)->rotation();
    camera.get<math::Transform>(TransformComponent)->position(player.get<math::Transform>(TransformComponent)->position() + bearing * staringOffset);
    camera.get<math::Transform>(TransformComponent)->rotation(bearing * staringRotation);
  }

private:
  float followDistance;

  Vector3 staringOffset;

  Quaternion staringRotation;

  ecs::Entity camera;
};

void addCamera(scene::Scene& scene) {
  auto cameraEntity = scene.entities().create();
  auto camTransform = cameraEntity.assign<math::Transform>(TransformComponent);
  camTransform->position({0, 15, -20});
  camTransform->rotation({5, 0, 0});

  cameraEntity.assign<render::PerspectiveCamera>(PerspectiveCameraComponent);
  scene.systems().create<ThirdPersonCamera>(cameraEntity);
}
