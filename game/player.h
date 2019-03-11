#ifndef PLAYER_H
#define PLAYER_H

#include "engine.h"

using namespace wage;

class Player {
};

class PlayerMovement : public System {

public:
  PlayerMovement() : System("PlayerMovement"), lastPos(Vector2()), mouseSpeed(0.0005) {
  }

  void start(SystemContext* context) {
    lastPos = Core::Instance->get<Input>()->mousePosition();
  }

  void fixedUpdate(SystemContext* context) {
    auto player = *Core::Instance->get<EntityManager>()->registry()->with<Player>().begin();
    auto body = player.get<RigidBody>();
    auto mousePos = Core::Instance->get<Input>()->mousePosition();
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;
    auto torque = Vector3::Up * mouseSpeed * dx + Vector3::Right * mouseSpeed * -dy;
    body->addTorqueImpulse(torque);
    auto bearing = player.get<Transform>()->rotation();
    float force = 0.1;
    auto impulse = Vector3::Zero;
    if (Core::Instance->get<Input>()->isPressed(Key::w)) {
      impulse += (bearing * Vector(0, 0, 1)).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::s)) {
      body->addImpulse((bearing * Vector(0, 0, -1)).normalized() * force);
    }
    if (Core::Instance->get<Input>()->isPressed(Key::a)) {
      impulse += (bearing * Vector(1, 0, 0)).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::d)) {
      impulse += (bearing * Vector(-1, 0, 0)).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::space)) {
      impulse += (bearing * Vector(0, 1, 0)).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::leftShift)) {
      impulse += (bearing * Vector(0, -1, 0)).normalized() * force;
    }
    body->addImpulse(impulse);

    if (Core::Instance->get<Input>()->isPressed(Key::f)) {
      body->shouldStop(true);
    }
    lastPos = mousePos;
  }

private:
  Vector2 lastPos;

  float mouseSpeed;
};

Entity addPlayer(EntityManager* entityManager, SystemManager* systemManager) {
  auto player = entityManager->create();
  auto transform = player.assign<Transform>();
  transform->position(Vector(0, 20, 0));
  transform->localScale(Vector(5, 5, 5));
  player.assign<RigidBody>(0.01);
  player.assign<Mesh>(Mesh::Sphere);
  player.assign<Collider>(ColliderType::sphere);
  player.assign<Material>(Texture("textures/test_planet.png"));
  player.assign<Player>();
  return player;
}

#endif //PLAYER_H