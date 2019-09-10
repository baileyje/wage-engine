#ifndef PLAYER_H
#define PLAYER_H

#include "engine.h"

using namespace wage;
using namespace wage::math;

class Player {
};

class PlayerMovement : public ecs::System {

public:
  PlayerMovement() : System(), lastPos(Vector2()), mouseSpeed(0.0001) {
  }

  void start(const ecs::SystemContext& context) {
    lastPos = core::Core::Instance->get<input::Input>()->mousePosition();
  }

  void fixedUpdate(const ecs::SystemContext& context) {
    auto player = *core::Core::Instance->get<ecs::EntityManager>()->with<Player>().begin();
    auto body = player.get<physics::RigidBody>();
    auto input = core::Core::Instance->get<input::Input>();
    auto mousePos = input->mousePosition();
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;
    auto bearing = player.get<math::Transform>()->rotation();
    auto torque = bearing * Vector3::Up * mouseSpeed * dx + bearing * Vector3::Right * mouseSpeed * -dy;
    body->addTorqueImpulse(torque);
    float force = 1.0;
    auto impulse = Vector3::Zero;
    if (input->isPressed(input::Key::w)) {
      impulse += (bearing * Vector::Forward).normalized() * force;
    }
    if (input->isPressed(input::Key::s)) {
      body->addImpulse((bearing * Vector::Backward).normalized() * force);
    }
    if (input->isPressed(input::Key::a)) {
      impulse += (bearing * Vector::Right).normalized() * force;
    }
    if (input->isPressed(input::Key::d)) {
      impulse += (bearing * Vector::Left).normalized() * force;
    }
    if (input->isPressed(input::Key::space)) {
      impulse += (bearing * Vector::Up).normalized() * force;
    }
    if (input->isPressed(input::Key::leftShift)) {
      impulse += (bearing * Vector::Down).normalized() * force;
    }
    body->addForce(impulse);

    if (input->isPressed(input::Key::f)) {
      body->shouldStop(true);
    }
    lastPos = mousePos;
  }

private:
  Vector2 lastPos;

  float mouseSpeed;
};

Entity addPlayer(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  auto mesh = render::Mesh::load("resources/meshes/player.obj");
  auto player = entityManager->create();
  auto transform = player.assign<math::Transform>(Vector(0, 0, 0), Vector(5, 5, 5), Vector(0, 5, 0));
  // transform.position();
  // transform.localScale();
  // transform.rotation();
  player.assign<physics::RigidBody>(0.01);
  player.assign<render::Mesh>(mesh);
  player.assign<physics::Collider>(physics::ColliderType::sphere);
  player.assign<render::Material>(render::Texture("textures/default.png"));
  player.assign<Player>();
  return player;
}

#endif //PLAYER_H