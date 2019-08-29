#ifndef PLAYER_H
#define PLAYER_H

#include "engine.h"

using namespace wage;

class Player {
};

class PlayerMovement : public System {

public:
  PlayerMovement() : System(), lastPos(Vector2()), mouseSpeed(0.0001) {
  }

  void start(const SystemContext& context) {
    lastPos = Core::Instance->get<Input>()->mousePosition();
  }

  void fixedUpdate(const SystemContext& context) {
    auto player = *Core::Instance->get<EntityManager>()->with<Player>().begin();
    auto body = player.get<RigidBody>();
    auto mousePos = Core::Instance->get<Input>()->mousePosition();
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;
    auto bearing = player.get<Transform>()->rotation();
    auto torque = bearing * Vector3::Up * mouseSpeed * dx + bearing * Vector3::Right * mouseSpeed * -dy;
    body->addTorqueImpulse(torque);
    float force = 1.0;
    auto impulse = Vector3::Zero;
    if (Core::Instance->get<Input>()->isPressed(Key::w)) {
      impulse += (bearing * Vector::Forward).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::s)) {
      body->addImpulse((bearing * Vector::Backward).normalized() * force);
    }
    if (Core::Instance->get<Input>()->isPressed(Key::a)) {
      impulse += (bearing * Vector::Right).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::d)) {
      impulse += (bearing * Vector::Left).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::space)) {
      impulse += (bearing * Vector::Up).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::leftShift)) {
      impulse += (bearing * Vector::Down).normalized() * force;
    }
    //printf("I: %f:%f:%f -> %f\n", impulse.x, impulse.y, impulse.z, sqrt(impulse.x * impulse.x + impulse.y * impulse.y + impulse.z * impulse.z ));
    body->addForce(impulse);

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
  auto mesh = Mesh::load("resources/meshes/player.obj");
  auto player = entityManager->create();
  auto transform = player.assign<Transform>(Vector(0, 0, 0), Vector(5, 5, 5), Vector(0, 5, 0));
  // transform.position();
  // transform.localScale();
  // transform.rotation();
  player.assign<RigidBody>(0.01);
  player.assign<Mesh>(mesh);
  player.assign<Collider>(ColliderType::sphere);
  player.assign<Material>(Texture("textures/default.png"));
  player.assign<Player>();
  return player;
}

#endif //PLAYER_H