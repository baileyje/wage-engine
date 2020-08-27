#ifndef CAMERA_H
#define CAMERA_H

#include "engine.h"

using namespace wage;

// This is a fairly lame third person rigging
class ThirdPersonCamera : public ecs::System {

public:
  ThirdPersonCamera(ecs::Entity camera) : System(), camera(camera) {
  }

  void start(const ecs::SystemContext& context) {
    auto manager = core::Core::Instance->get<ecs::EntityManager>();
    auto player = *manager->with({PlayerComponent}).begin();
    auto cameraPosition = camera.get<math::Transform>(TransformComponent)->position();
    auto playerPosition = player.get<math::Transform>(TransformComponent)->position();
    followDistance = cameraPosition.distanceTo(playerPosition);
    staringOffset = cameraPosition - playerPosition;
    staringRotation = camera.get<math::Transform>(TransformComponent)->rotation();
  }

  void update(const ecs::SystemContext& context) {
    // Adjust camera position base on player
    auto manager = core::Core::Instance->get<ecs::EntityManager>();
    auto player = *manager->with({PlayerComponent}).begin();
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

void addCamera(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  auto cameraEntity = entityManager->create();
  auto camTransform = cameraEntity.assign<math::Transform>(TransformComponent);
  camTransform->position({0, 20, -30});
  camTransform->rotation({20, 0, 0});
  cameraEntity.assign<component::PerspectiveCamera>(PerspectiveCameraComponent);
  systemManager->create<ThirdPersonCamera>(cameraEntity);
}

#endif // CAMERA_H