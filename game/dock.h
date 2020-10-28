#pragma once

#include "engine.h"

using namespace wage;

#define DockComponent 2003

void addRandomDock(scene::Scene* const scene);

class Dock {
};

void addDock(scene::Scene& scene, Vector position, float scale) {
  auto entity = scene.entities().create();
  auto transform = entity.assign<math::Transform>(TransformComponent);
  transform->localPosition = position;
  transform->localScale = Vector(scale, scale, scale);
  entity.assign<render::MeshSpec>(MeshComponent, "dock.obj");
  entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("odd_space_2.png"));
  entity.assign<physics::RigidBody>(RigidBodyComponent, 0.01, physics::RigidBody::Type::immovable);
  entity.assign<physics::Collider>(ColliderComponent, physics::Collider::Type::box);
  entity.assign<Enemy>(EnemyComponent);
}

void addRandomDock(scene::Scene& scene) {
  float x = randomBetween(-20000, 20000);
  float y = randomBetween(-20000, 20000);
  float z = randomBetween(-20000, 20000);
  // float scale = (rand() % 100) / 30.0;
  addDock(scene, Vector(x, y, z) * 2, 50 /*scale*/);
}
