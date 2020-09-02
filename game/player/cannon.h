#pragma once

#include "engine.h"

using namespace wage;
using namespace wage::math;

#define CannonComponent 2004
#define CannonBallComponent 2005

class Cannon {
public:
  Cannon(Vector3 attachmentPoint, float power, float rate) : attachmentPoint(attachmentPoint), power(power), rate(rate) {}

  Vector3 attachmentPoint;

  float power;

  float rate;

  float lastFire = 0;
};

class CannonBall {
};

class CannonControl : public ecs::System {

public:
  CannonControl() : System() {
  }

  void start(const ecs::SystemContext& context) {
  }

  void update(const ecs::SystemContext& context) {
    auto entityManager = core::Core::Instance->get<ecs::EntityManager>();
    auto input = core::Core::Instance->get<input::Input>();
    if (input->isPressed(input::MouseButton::One)) {
      for (auto entity : entityManager->with({CannonComponent})) {
        auto cannon = entity.get<Cannon>(CannonComponent);
        auto delay = 1 / cannon->rate;
        if (context.time() > cannon->lastFire + delay) {
          fire(entity, cannon);
          cannon->lastFire = context.time();
        }
      }
    }
    auto physics = core::Core::Instance->get<physics::Physics>();
    for (auto entity : entityManager->with({CannonBallComponent})) {
      auto collisions = physics->collisionsFor(entity);
      if (collisions.size() > 0) {
        std::cout << "Cannon Ball Collisions: " << collisions.size() << " \n";
        for (auto collision : collisions) {
          entityManager->destroy(collision.otherEntity());
        }
      }
    }
  }

  void fire(ecs::Entity entity, Cannon* cannon) {
    auto entityManager = core::Core::Instance->get<ecs::EntityManager>();
    auto ball = entityManager->create();
    auto entityTransform = entity.get<math::Transform>(TransformComponent);
    auto bearing = entity.get<math::Transform>(TransformComponent)->rotation();
    auto ballPosition = entityTransform->position() + (bearing * Vector::Forward).normalized() * 4;
    ball.assign<math::Transform>(TransformComponent, ballPosition, Vector(1, 1, 1), Vector(0, 0, 0));
    ball.assign<physics::RigidBody>(RigidBodyComponent, 0.1);
    ball.assign<render::MeshSpec>(MeshComponent, render::MeshSpec::Sphere);
    ball.assign<physics::Collider>(ColliderComponent, physics::ColliderType::sphere);
    ball.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("mover.png"));
    ball.assign<CannonBall>(CannonBallComponent);
    ball.get<physics::RigidBody>(RigidBodyComponent)->addForce((bearing * Vector::Forward).normalized() * cannon->power);
  }
};

void addCannonTo(ecs::Entity entity, ecs::SystemManager* systemManager) {
  entity.assign<Cannon>(CannonComponent, Vector3{0, 0, 10}, 800, 10);
}
