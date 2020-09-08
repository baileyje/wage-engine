#pragma once

#include "engine.h"

#include "player.h"

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
    auto& entities = scene::Scene::current().entities();
    auto input = core::Core::Instance->get<input::Input>();
    if (input->isPressed(input::MouseButton::One)) {
      for (auto entity : entities.with({CannonComponent})) {
        fire(context, entity, entity.get<Cannon>(CannonComponent));
      }
    }
    auto physics = core::Core::Instance->get<physics::Physics>();
    auto player = *entities.with({2000}).begin();
    for (auto entity : entities.with({CannonBallComponent})) {
      auto collisions = physics->collisionsFor(entity);
      if (collisions.size() > 0) {
        // std::cout << "Cannon Ball Collisions: " << collisions.size() << " \n";
        for (auto collision : collisions) {
          if (collision.otherEntity().id() == player.id()) {
            std::cout << "Skipping Player\n";
            continue;
          }
          entities.destroy(collision.otherEntity());
          entities.destroy(collision.entity());
          audio::ClipSpec spec("bounce.wav");
          core::Core::Instance->get<audio::Audio>()->play(spec);
        }
      }
    }
  }

  void aim(const ecs::SystemContext& context, ecs::Entity entity, Cannon* cannon) {
    auto physics = core::Core::Instance->get<physics::Physics>();
    auto transform = entity.get<math::Transform>(TransformComponent);
    auto bearing = transform->rotation();
    auto startPosition = transform->position() + (bearing * Vector::Forward).normalized() * 5;
    auto endPosition = transform->position() + (bearing * Vector::Forward).normalized() * 20000;
    auto results = physics->castRay(startPosition, endPosition);
    if (!results.empty()) {

      fire(context, entity, cannon);
    }
  }

  void fire(const ecs::SystemContext& context, ecs::Entity entity, Cannon* cannon) {
    auto delay = 1 / cannon->rate;
    if (context.time() > cannon->lastFire + delay) {
      auto& entities = scene::Scene::current().entities();
      auto ball = entities.create();
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

      cannon->lastFire = context.time();
    }
  }
};

void addCannonTo(ecs::Entity entity) {
  entity.assign<Cannon>(CannonComponent, Vector3{0, 0, 10}, 1000, 10);
}
