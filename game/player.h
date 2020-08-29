#ifndef PLAYER_H
#define PLAYER_H

#include "engine.h"

using namespace wage;
using namespace wage::math;

#define PlayerComponent 2000

class Player {
};

class PlayerPhysicsMovement : public ecs::System {

public:
  PlayerPhysicsMovement() : System(), lastPos(Vector2()), mouseSpeed(0.0001) {
  }

  void start(const ecs::SystemContext& context) {
    lastPos = core::Core::Instance->get<input::Input>()->mousePosition();
  }

  void fixedUpdate(const ecs::SystemContext& context) {
    auto player = *core::Core::Instance->get<ecs::EntityManager>()->with({PlayerComponent, RigidBodyComponent, TransformComponent}).begin();
    auto body = player.get<physics::RigidBody>(RigidBodyComponent);
    auto input = core::Core::Instance->get<input::Input>();
    auto mousePos = input->mousePosition();
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;
    auto bearing = player.get<math::Transform>(TransformComponent)->rotation();
    auto torque = bearing * Vector3::Up * mouseSpeed * dx + bearing * Vector3::Right * mouseSpeed * -dy;
    body->addTorqueImpulse(torque);
    float force = 2.0;
    auto impulse = Vector3::Zero;
    if (input->isPressed(input::Key::w)) {
      impulse += (bearing * Vector::Forward).normalized() * force;
    }
    if (input->isPressed(input::Key::s)) {
      impulse += (bearing * Vector::Backward).normalized() * force;
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
    // if (input->isPressed(input::Key::zero)) {
    //   body->shouldStop(true);
    //   player.assign<physics::RigidBody>(RigidBodyComponent, 0.01, physics::RigidBodyType::kinematic);
    //   player.get<math::Transform>(TransformComponent)->position({0, 0, 0});
    // }
    lastPos = mousePos;
  }

private:
  Vector2 lastPos;

  float mouseSpeed;
};

class PlayerBasicMovement : public ecs::System {

public:
  PlayerBasicMovement() : System(), lastPos(Vector2()) {
  }

  void start(const ecs::SystemContext& context) {
    lastPos = core::Core::Instance->get<input::Input>()->mousePosition();
  }

  void fixedUpdate(const ecs::SystemContext& context) {
    auto player = *core::Core::Instance->get<ecs::EntityManager>()->with({PlayerComponent, TransformComponent}).begin();
    auto input = core::Core::Instance->get<input::Input>();
    auto mousePos = input->mousePosition();
    // auto dx = lastPos.x - mousePos.x;
    // auto dy = lastPos.y - mousePos.y;
    auto bearing = player.get<math::Transform>(TransformComponent)->rotation();
    // auto torque = bearing * Vector3::Up * mouseSpeed * dx + bearing * Vector3::Right * mouseSpeed * -dy;

    // body->addTorqueImpulse(torque);
    float force = 180.0;
    auto impulse = Vector3::Zero;
    if (input->isPressed(input::Key::w)) {
      impulse += (bearing * Vector::Forward).normalized() * force;
    }
    if (input->isPressed(input::Key::s)) {
      impulse += (bearing * Vector::Backward).normalized() * force;
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

    player.get<math::Transform>(TransformComponent)->position(player.get<math::Transform>(TransformComponent)->position() + impulse);

    // body->addForce(impulse);

    if (input->isPressed(input::Key::f)) {
      // body->shouldStop(true);
    }
    lastPos = mousePos;
  }

private:
  Vector2 lastPos;

  // float mouseSpeed;
};

ecs::Entity addPlayer(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  auto player = entityManager->create();
  player.assign<math::Transform>(TransformComponent, Vector(0, 0, 0), Vector(5, 5, 5), Vector(0, 5, 0));
  player.assign<physics::RigidBody>(RigidBodyComponent, 0.01);
  player.assign<render::MeshSpec>(MeshComponent, "player.obj");
  player.assign<physics::Collider>(ColliderComponent, physics::ColliderType::sphere);
  player.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec());
  player.assign<Player>(PlayerComponent);
  return player;
}

#endif //PLAYER_H