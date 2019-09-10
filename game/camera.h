#ifndef CAMERA_H
#define CAMERA_H

#include "engine.h"

using namespace wage;


// This is a fairly lame third person rigging
class ThirdPersonCamera : public ecs::System {

public:
  ThirdPersonCamera(Entity camera) : System(), camera(camera) {
  }

  void start(const ecs::SystemContext& context) {
    auto manager = core::Core::Instance->get<ecs::EntityManager>();
    auto player = *manager->with<Player>().begin();
    followDistance = camera.get<math::Transform>()->position().distanceTo(player.get<math::Transform>()->position());
    staringOffset = camera.get<math::Transform>()->position() - player.get<math::Transform>()->position();
    staringRotation = camera.get<math::Transform>()->rotation();
  }

  void update(const ecs::SystemContext& context) {
    // Adjust camera position base on player
    auto manager = core::Core::Instance->get<ecs::EntityManager>();
    auto player = *manager->with<Player>().begin();
    auto bearing = player.get<math::Transform>()->rotation();
    // auto reverseBearing = (staringBearing * bearing * Vector(0, 0, -1)).normalized();
    // auto pos = player.get<Transform>()->position() + reverseBearing * followDistance;
    // camera.get<Transform>()->position(pos);
    // camera.get<Transform>()->rotation(staringBearing * bearing);
    camera.get<math::Transform>()->position(player.get<math::Transform>()->position() + bearing * staringOffset);
    camera.get<math::Transform>()->rotation(bearing * staringRotation);
  }

private:

  float followDistance;

  Vector3 staringOffset;

  Quaternion staringRotation;

  Entity camera;

};

void addCamera(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  auto cameraEntity = entityManager->create();
  auto camTransform = cameraEntity.assign<math::Transform>();
  camTransform->position({0, 20, -30});
  camTransform->rotation({20, 0, 0});
  cameraEntity.assign<component::PerspectiveCamera>();
  systemManager->create<ThirdPersonCamera>(cameraEntity);
}

#endif // CAMERA_H