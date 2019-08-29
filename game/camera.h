#ifndef CAMERA_H
#define CAMERA_H

#include "engine.h"

using namespace wage;


// This is a fairly lame third person rigging
class ThirdPersonCamera : public System {

public:
  ThirdPersonCamera(Entity camera) : System(), camera(camera) {
  }

  void start(const SystemContext& context) {
    auto manager = Core::Instance->get<EntityManager>();
    auto player = *manager->with<Player>().begin();
    followDistance = camera.get<Transform>()->position().distanceTo(player.get<Transform>()->position());
    staringOffset = camera.get<Transform>()->position() - player.get<Transform>()->position();
    staringRotation = camera.get<Transform>()->rotation();
  }

  void update(const SystemContext& context) {
    // Adjust camera position base on player
    auto manager = Core::Instance->get<EntityManager>();
    auto player = *manager->with<Player>().begin();
    auto bearing = player.get<Transform>()->rotation();
    // auto reverseBearing = (staringBearing * bearing * Vector(0, 0, -1)).normalized();
    // auto pos = player.get<Transform>()->position() + reverseBearing * followDistance;
    // camera.get<Transform>()->position(pos);
    // camera.get<Transform>()->rotation(staringBearing * bearing);
    camera.get<Transform>()->position(player.get<Transform>()->position() + bearing * staringOffset);
    camera.get<Transform>()->rotation(bearing * staringRotation);
  }

private:

  float followDistance;

  Vector3 staringOffset;

  Quaternion staringRotation;

  Entity camera;

};

void addCamera(EntityManager* entityManager, SystemManager* systemManager) {
  auto cameraEntity = entityManager->create();
  auto camTransform = cameraEntity.assign<Transform>();
  camTransform->position({0, 20, -30});
  camTransform->rotation({20, 0, 0});
  cameraEntity.assign<PerspectiveCamera>();
  systemManager->create<ThirdPersonCamera>(cameraEntity);
}

#endif // CAMERA_H