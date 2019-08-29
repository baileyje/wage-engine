#ifndef ENEMY_H
#define ENEMY_H

#include "engine.h"

using namespace wage;

void addRandomEnemy(EntityManager* entityManager, SystemManager* systemManager);

class Enemy {
};

class EnemyMovement : public System {

public:
  EnemyMovement() : System(), chasing(false), running(false) {
  }
  void fixedUpdate(const SystemContext& context) {
    auto manager = Core::Instance->get<EntityManager>();
    if (Core::Instance->get<Input>()->isPressed(Key::c) && !chasing) {
      for (auto entity : manager->with<Enemy>()) {
        entity.get<RigidBody>()->shouldStop(true);
      }
      chasing = true;
      running = false;
      return;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::r) && !running) {
      for (auto entity : manager->with<Enemy>()) {
        entity.get<RigidBody>()->shouldStop(true);
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
        if (chasing && Vector::distance(target.get<Transform>()->position(), entity.get<Transform>()->position()) < 20) {
          entity.get<RigidBody>()->shouldStop(true);
          continue;
        }
        auto dir = target.get<Transform>()->position() - entity.get<Transform>()->position();
        auto impulse = dir.normalized() * 0.1;
        if (running) {
          impulse *= -1;
        }
        entity.get<RigidBody>()->addImpulse(impulse);
      }
    }
  }

private:
  bool chasing;

  bool running;
};

class EnemyLauncher : public System {

public:
  EnemyLauncher() : System() {
  }

  void start(const SystemContext& context) {
    lastLaunch = context.time();
  }

  void update(const SystemContext& context) {
    if (Core::Instance->get<Input>()->isPressed(Key::n) && context.time() - lastLaunch > launchThreshold) {
      auto manager = Core::Instance->get<EntityManager>();
      auto systemManager = Core::Instance->get<SystemManager>();
      addRandomEnemy(manager, systemManager);
      lastLaunch = context.time();
    }
  }

private:

  double lastLaunch = 0;

  double launchThreshold = 0.001;

};

void addEnemy(EntityManager* entityManager, SystemManager* systemManager, Vector position, float scale) {
  Entity entity = entityManager->create();
  auto transform = entity.assign<Transform>();
  transform->position(position);
  transform->localScale(Vector(scale, scale, scale));
  entity.assign<RigidBody>(0.001);
  entity.assign<Mesh>(Mesh::Cube);
  entity.assign<Collider>(ColliderType::box);
  entity.assign<Material>(Texture("textures/odd_space_2.png"));
  entity.assign<Enemy>();
}

void addRandomEnemy(EntityManager* entityManager, SystemManager* systemManager) {
  float x = rand() % 5000 - 2500;
  float y = rand() % 5000 - 2500;
  float z = rand() % 5000 - 2500;
  // float scale = (rand() % 100) / 30.0;
  addEnemy(entityManager, systemManager, Vector(x, y, z) * 2, 5 /*scale*/);
}

#endif // ENEMY_H