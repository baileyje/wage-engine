#pragma once

#include "engine.h"

using namespace wage;

#define DockComponent 2001

void addRandomDock(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager);

class Dock {
};

void addDock(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager, Vector position, float scale) {
  auto entity = entityManager->create();
  auto transform = entity.assign<math::Transform>(TransformComponent);
  transform->position(position);
  transform->localScale(Vector(scale, scale, scale));
  entity.assign<render::MeshSpec>(MeshComponent, "dock.obj");
  entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("odd_space_2.png"));
  entity.assign<physics::RigidBody>(RigidBodyComponent, 0.01, physics::RigidBodyType::immovable);
  entity.assign<physics::Collider>(ColliderComponent, physics::ColliderType::box);
  entity.assign<Enemy>(EnemyComponent);
}

void addRandomDock(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  float x = rand() % 2000 - 1000;
  float y = rand() % 2000 - 1000;
  float z = rand() % 2000 - 1000;
  // float scale = (rand() % 100) / 30.0;
  addDock(entityManager, systemManager, Vector(x, y, z) * 2, 50 /*scale*/);
}
