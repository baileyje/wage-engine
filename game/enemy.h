#ifndef ENEMY_H
#define ENEMY_H

#include "engine.h"

using namespace wage;

class Enemy {
};

class EnemyMovement : public System {

public:
  EnemyMovement() : System("EnemyMovement"), chasing(false), running(false) {
  }
  void fixedUpdate(SystemContext* context) {
    auto manager = Core::Instance->get<EntityManager>();
    if (Core::Instance->get<Input>()->isPressed(Key::c) && !chasing) {
      for (auto entity : manager->registry()->with<Enemy>()) {
        entity.get<RigidBody>()->shouldStop(true);
      }
      chasing = true;
      running = false;
      return;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::r) && !running) {
      for (auto entity : manager->registry()->with<Enemy>()) {
        entity.get<RigidBody>()->shouldStop(true);
      }
      running = true;
      chasing = false;
    }

    auto target = *manager->registry()->with<Player>().begin();
    if (running || chasing) {
      for (auto entity : manager->registry()->with<Enemy>()) {
        if (!entity.valid()) {
          return;
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
  float x = rand() % 400 - 200;
  float y = rand() % 400 - 200;
  float z = rand() % 400 - 200;
  float scale = (rand() % 100) / 30.0;
  addEnemy(entityManager, systemManager, Vector(x, y, z) * 2, 1 /*scale*/);
}

#endif // ENEMY_H