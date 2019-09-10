#ifndef ENEMY_H
#define ENEMY_H

#include "engine.h"

using namespace wage;

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
      for (auto entity : manager->with<Enemy>()) {
        entity.get<physics::RigidBody>()->shouldStop(true);
      }
      chasing = true;
      running = false;
      return;
    }
    if (input->isPressed(input::Key::r) && !running) {
      for (auto entity : manager->with<Enemy>()) {
        entity.get<physics::RigidBody>()->shouldStop(true);
      }
      running = true;
      chasing = false;
    }

    auto target = *manager->with<Player>().begin();
    if (running || chasing) {
      for (auto entity : manager->with<Enemy>()) {
        if (!entity.valid()) {
          continue;
        }
        if (chasing && Vector::distance(target.get<math::Transform>()->position(), entity.get<math::Transform>()->position()) < 20) {
          entity.get<physics::RigidBody>()->shouldStop(true);
          continue;
        }
        auto dir = target.get<math::Transform>()->position() - entity.get<Transform>()->position();
        auto impulse = dir.normalized() * 0.1;
        if (running) {
          impulse *= -1;
        }
        entity.get<physics::RigidBody>()->addImpulse(impulse);
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
  Entity entity = entityManager->create();
  auto transform = entity.assign<math::Transform>();
  transform->position(position);
  transform->localScale(Vector(scale, scale, scale));
  entity.assign<physics::RigidBody>(0.001);
  entity.assign<render::Mesh>(render::Mesh::Cube);
  entity.assign<physics::Collider>(physics::ColliderType::box);
  entity.assign<render::Material>(render::Texture("textures/odd_space_2.png"));
  entity.assign<Enemy>();
}

void addRandomEnemy(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  float x = rand() % 5000 - 2500;
  float y = rand() % 5000 - 2500;
  float z = rand() % 5000 - 2500;
  // float scale = (rand() % 100) / 30.0;
  addEnemy(entityManager, systemManager, Vector(x, y, z) * 2, 5 /*scale*/);
}

#endif // ENEMY_H