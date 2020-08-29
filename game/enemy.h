#ifndef ENEMY_H
#define ENEMY_H

#include "engine.h"

using namespace wage;

#define EnemyComponent 2001

void addRandomEnemy(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager);

class Enemy {
};

class EnemyMovement : public ecs::System {

public:
  EnemyMovement() : System(), chasing(false), running(false) {
  }

  void fixedUpdate(const ecs::SystemContext& context) {
    auto manager = core::Core::Instance->get<ecs::EntityManager>();
    auto input = core::Core::Instance->get<input::Input>();
    if (input->isPressed(input::Key::c) && !chasing) {
      for (auto entity : manager->with({EnemyComponent})) {
        entity.get<physics::RigidBody>(RigidBodyComponent)->shouldStop(true);
      }
      chasing = true;
      running = false;
      return;
    }
    if (input->isPressed(input::Key::r) && !running) {
      for (auto entity : manager->with({EnemyComponent})) {
        entity.get<physics::RigidBody>(RigidBodyComponent)->shouldStop(true);
      }
      running = true;
      chasing = false;
    }

    auto target = *manager->with({PlayerComponent}).begin();
    if (running || chasing) {
      for (auto enemy : manager->with({EnemyComponent})) {
        if (!enemy.valid()) {
          continue;
        }
        auto enemyPosition = enemy.get<math::Transform>(TransformComponent)->position();
        auto targetPosition = target.get<math::Transform>(TransformComponent)->position();
        if (chasing && Vector::distance(targetPosition, enemyPosition) < 20) {
          enemy.get<physics::RigidBody>(RigidBodyComponent)->shouldStop(true);
          continue;
        }
        auto dir = targetPosition - enemyPosition;
        auto impulse = dir.normalized() * 100.0;
        if (running) {
          impulse *= -1;
        }
        enemy.get<physics::RigidBody>(RigidBodyComponent)->addImpulse(impulse);
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
      auto manager = core::Core::Instance->get<ecs::EntityManager>();
      auto systemManager = core::Core::Instance->get<ecs::SystemManager>();
      addRandomEnemy(manager, systemManager);
      lastLaunch = context.time();
    }
  }

private:
  double lastLaunch = 0;

  double launchThreshold = 0.001;
};

void addEnemy(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager, Vector position, float scale) {
  auto entity = entityManager->create();
  auto transform = entity.assign<math::Transform>(TransformComponent);
  transform->position(position);
  transform->localScale(Vector(scale, scale, scale));
  entity.assign<render::MeshSpec>(MeshComponent, render::MeshSpec::Cube);
  entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("odd_space_2.png"));
  entity.assign<physics::RigidBody>(RigidBodyComponent, 0.01);
  entity.assign<physics::Collider>(ColliderComponent, physics::ColliderType::box);
  entity.assign<Enemy>(EnemyComponent);
}

void addRandomEnemy(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  float x = randomBetween(-20000, 20000);
  float y = randomBetween(-20000, 20000);
  float z = randomBetween(-20000, 20000);
  addEnemy(entityManager, systemManager, Vector(x, y, z) * 2, 5 /*scale*/);
}

#endif // ENEMY_H