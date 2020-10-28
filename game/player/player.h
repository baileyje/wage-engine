#pragma once

#include "engine.h"
#include "cannon.h"
#include "debug.h"

using namespace wage;
using namespace wage::math;

#define PlayerComponent 2000

class Player {
};

class PlayerPhysicsMovement : public ecs::System {

public:
  PlayerPhysicsMovement() : System(), lastPos(Vector2()), mouseSpeed(2) {
  }

  void start(const ecs::SystemContext& context) {
    lastPos = core::Core::Instance->get<input::Input>()->mousePosition();
  }

  void fixedUpdate(const ecs::SystemContext& context) {
    auto player = *scene::Scene::current().entities().with({PlayerComponent, RigidBodyComponent, TransformComponent}).begin();
    if (!player.valid()) return;
    auto body = player.get<physics::RigidBody>(RigidBodyComponent);
    auto input = core::Core::Instance->get<input::Input>();
    auto mousePos = input->mousePosition();
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;
    auto bearing = player.get<math::Transform>(TransformComponent)->localRotation;
    auto torque = bearing * Vector3::Up * mouseSpeed * dx + bearing * Vector3::Right * mouseSpeed * -dy;

    if (input->isPressed(input::Key::q)) {
      torque += (bearing * Vector3::Forward).normalized() * Vector3(0, 0, -1); //bearing * Vector3::Up * mouseSpeed * dx + bearing * Vector3::Right * mouseSpeed * -dy;
    }
    if (input->isPressed(input::Key::e)) {
      torque += (bearing * Vector3::Forward).normalized() * Vector3(0, 0, 1);
    }
    body->addTorqueImpulse(torque);

    float force = 10000.0;
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
    if (input->isPressed(input::Key::zero)) {
      body->shouldStop(true);
      // player.assign<physics::RigidBody>(RigidBodyComponent, 0.01, physics::RigidBody::Type::kinematic);
      player.get<math::Transform>(TransformComponent)->position({0, 0, 0});
    }
    lastPos = mousePos;

    auto physics = core::Core::Instance->get<physics::Physics>();
    if (physics->collisionsFor(player).size() > 0)
      std::cout << "Player Collisions: " << physics->collisionsFor(player).size() << " \n";
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
    auto player = *scene::Scene::current().entities().with({PlayerComponent, TransformComponent}).begin();
    auto input = core::Core::Instance->get<input::Input>();
    auto mousePos = input->mousePosition();
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;
    auto bearing = player.get<math::Transform>(TransformComponent)->localRotation;
    auto newDir = bearing * Vector3::Up * 1 * dx + bearing * Vector3::Right * 1 * -dy;
    player.get<math::Transform>(TransformComponent)->rotation({newDir});

    // body->addTorqueImpulse(torque);
    float force = 1.0;
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

    player.get<math::Transform>(TransformComponent)->position(player.get<math::Transform>(TransformComponent)->localPosition + impulse);

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

ecs::Entity addPlayer(scene::Scene& scene) {
  auto player = IDCHECK(scene.entities().create());
  auto transform = player.assign<math::Transform>(TransformComponent, Vector(0, 5, 0), Vector(5, 5, 5), Vector(-3, 0, 0));
  player.assign<physics::RigidBody>(RigidBodyComponent, 100);
  auto mesh = player.assign<render::MeshSpec>(MeshComponent, "player.obj");
  mesh->transform.localPosition = { 0,0,0.2 };
  // mesh->transform.localScale = { 1, 1, 1 };
  auto collider = player.assign<physics::Collider>(ColliderComponent, physics::Collider::Type::sphere);
  collider->transform.localScale = {2, 2, 2};
  // collider->transform.localPosition = {0, 0, 10};m
  player.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("odd_space.png"));
  player.assign<Player>(PlayerComponent);
  addCannonTo(player);
  return player;
}
