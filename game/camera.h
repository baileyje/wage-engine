#ifndef CAMERA_H
#define CAMERA_H

#include "engine.h"

using namespace wage;


// This is a fairly lame third person rigging
class ThirdPersonCamera : public System {

public:
  ThirdPersonCamera(Entity camera, Entity player) : System("ThirdPersonCamera"), camera(camera), player(player) {
  }

  void start(SystemContext* context) {
    followDistance = camera.get<Transform>()->position().distanceTo(player.get<Transform>()->position());
  }

  void update(SystemContext* context) {
    // . Adjust camera position
    auto bearing = player.get<Transform>()->rotation();
    auto reverseBearing = (bearing * Vector(0, 0, -1)).normalized();
    auto pos = player.get<Transform>()->position() + reverseBearing * followDistance;
    camera.get<Transform>()->position(pos);
    camera.get<Transform>()->rotation(bearing);
  }

private:
  float followDistance;

  Entity camera;

  Entity player;
};

#endif // CAMERA_H