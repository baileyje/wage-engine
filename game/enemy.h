#pragma once

#include "engine.h"
#include "debug.h"

using namespace wage;

#define EnemyComponent 2001

void addRandomEnemy(scene::Scene& scene);

class Enemy {
};

class EnemyMovement : public ecs::System {

public:
  EnemyMovement() : System(), chasing(false), running(false) {
  }

  void fixedUpdate(const ecs::SystemContext& context) {
    auto& entities = scene::Scene::current().entities();
    auto input = core::Core::Instance->get<input::Input>();
    if (input->isPressed(input::Key::c) && !chasing) {
      // for (auto entity : manager->with({EnemyComponent})) {
      //   entity.get<physics::RigidBody>(RigidBodyComponent)->shouldStop(true);
      // }
      chasing = true;
      running = false;
      return;
    }
    if (input->isPressed(input::Key::r) && !running) {
      for (auto entity : entities.with({EnemyComponent})) {
        entity.get<physics::RigidBody>(RigidBodyComponent)->shouldStop(true);
      }
      running = true;
      chasing = false;
    }

    auto target = *entities.with({PlayerComponent}).begin();
    if (running || chasing) {
      for (auto enemy : entities.with({EnemyComponent})) {
        if (!enemy.valid()) {
          continue;
        }
        auto enemyPosition = enemy.get<math::Transform>(TransformComponent)->position();
        auto targetPosition = target.get<math::Transform>(TransformComponent)->position();
        if (chasing && Vector::distance(targetPosition, enemyPosition) < 100) {
          enemy.get<physics::RigidBody>(RigidBodyComponent)->shouldStop(true);
          continue;
        }
        auto dir = targetPosition - enemyPosition;
        auto impulse = dir.normalized() * 1000.0;
        if (running) {
          impulse *= -1;
        }
        enemy.get<physics::RigidBody>(RigidBodyComponent)->addForce(impulse);
      }
    }
  }

private:
  bool chasing;

  bool running;
};

class EnemyLauncher : public ecs::System {

public:
  EnemyLauncher() : System() {
  }

  void start(const ecs::SystemContext& context) {
    lastLaunch = context.time();
  }

  void update(const ecs::SystemContext& context) {
    if (core::Core::Instance->get<input::Input>()->isPressed(input::Key::n) && context.time() - lastLaunch > launchThreshold) {
      addRandomEnemy(scene::Scene::current());
      lastLaunch = context.time();
    }
  }

private:
  double lastLaunch = 0;

  double launchThreshold = 0.001;
};

ecs::Entity addEnemy(scene::Scene& scene, Vector position, float scale) {
  auto entity = IDCHECK(scene.entities().create());
  auto transform = entity.assign<math::Transform>(TransformComponent);
  transform->position(position);
  transform->localScale(Vector(scale, scale, scale));
  entity.assign<render::MeshSpec>(MeshComponent, "cube.obj");
  entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("odd_space_2.png"));
  entity.assign<physics::RigidBody>(RigidBodyComponent, 5, physics::RigidBodyType::dynamic);
  entity.assign<physics::Collider>(ColliderComponent, physics::ColliderType::box);
  entity.assign<Enemy>(EnemyComponent);
  return entity;
}

void addRandomEnemy(scene::Scene& scene) {
  float x = randomBetween(-2000, 2000);
  float y = randomBetween(-2000, 2000);
  float z = randomBetween(-2000, 2000);
  addEnemy(scene, Vector(x, y, z) * 2, 5 /*scale*/);
}
