#pragma once

#include "engine.h"
#include "enemy.h"
#include "debug.h"

using namespace wage;

#define WallComponent 2006

class Wall {
};

ecs::Entity addBlock(scene::Scene& scene, Vector position, float scale) {
  auto entity = IDCHECK(scene.entities().create());
  auto transform = entity.assign<math::Transform>(TransformComponent);
  transform->localPosition = position;
  transform->localScale = Vector(scale, scale, scale);
  entity.assign<render::MeshSpec>(MeshComponent, "cube.obj");
  entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("odd_space_2.png"));
  entity.assign<physics::RigidBody>(RigidBodyComponent, 1000, physics::RigidBody::Type::immovable);
  entity.assign<physics::Collider>(ColliderComponent, physics::Collider::Type::box);
  return entity;
}

void addWall(scene::Scene &scene) {
  int width = 40;
  int height = 40;
  float margin = 0;
  float enemyScale = 5;
  float offsetX = -(enemyScale * width / 2);
  float offsetY = -(enemyScale * height / 2);

  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      auto enemy = addBlock(scene, math::Vector{offsetX, offsetY, 40}, enemyScale);
      enemy.assign<Wall>(WallComponent);
      offsetY += enemyScale + margin;
    }
    offsetX += enemyScale + margin;
    offsetY = -(enemyScale * height / 2);
  }
}

class WallSystem : public ecs::System {

  void update(const ecs::SystemContext &context) {
    // auto &entities = scene::Scene::current().entities();
    // auto physics = core::Core::Instance->get<physics::Physics>();
    // std::vector<job::Job> castJobs;
    // for (auto wallSegment : entities.with({WallComponent, TransformComponent, MaterialComponent})) {
    //   auto mat = wallSegment.get<render::MaterialSpec>(MaterialComponent);
    //   auto position = wallSegment.get<math::Transform>(TransformComponent)->position();
    //   // castJobs.push_back({[mat, position, physics]() {        
    //   //   auto hits = physics->castRay(position, position + Vector3{0, 0, -40});
    //   //   if (hits.size() > 0) {          
    //   //     mat->texture(render::TextureSpec("default.png"));
    //   //   } else {
    //   //     mat->texture(render::TextureSpec("odd_space_2.png"));
    //   //   }
    //   // }});
    // }
    // core::Core::Instance->get<job::Manager>()->dispatch(castJobs)->wait();
    /*
   for (auto wallSegment : entities.with({WallComponent, TransformComponent, MaterialComponent})) {
      auto mat = wallSegment.get<render::MaterialSpec>(MaterialComponent);
      auto position = wallSegment.get<math::Transform>(TransformComponent)->position();
      auto hits = physics->castRay(position, position + Vector3{0, 0, -40});
      if (hits.size() > 0) {          
        mat->texture(render::TextureSpec("default.png"));
      } else {
        mat->texture(render::TextureSpec("odd_space_2.png"));
      }
    }
    */
  }
};