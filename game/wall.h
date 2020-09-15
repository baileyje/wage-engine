#pragma once

#include "engine.h"
#include "enemy.h"
#include "debug.h"

using namespace wage;

#define WallComponent 2006

class Wall {
};

void addWall(scene::Scene &scene) {
  int width = 20;
  int height = 20;
  float margin = 0.1;
  float enemyScale = 5;
  float offsetX = -(enemyScale * width / 2);
  float offsetY = -(enemyScale * height / 2);

  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      auto enemy = addEnemy(scene, math::Vector{offsetX, offsetY, 40}, enemyScale);
      enemy.assign<Wall>(WallComponent);
      offsetY += enemyScale + margin;
    }
    offsetX += enemyScale + margin;
    offsetY = -(enemyScale * height / 2);
  }
}

class WallSystem : public ecs::System {

  void update(const ecs::SystemContext &context) {
    auto &entities = scene::Scene::current().entities();
    auto physics = core::Core::Instance->get<physics::Physics>();
    std::vector<job::Job> castJobs;
    for (auto wallSegment : entities.with({WallComponent, TransformComponent, MaterialComponent})) {
      auto mat = wallSegment.get<render::MaterialSpec>(MaterialComponent);
      auto position = wallSegment.get<math::Transform>(TransformComponent)->position();
      castJobs.push_back({[mat, position, physics]() {        
        auto hits = physics->castRay(position, position + Vector3{0, 0, -40});
        if (hits.size() > 0) {          
          mat->texture(render::TextureSpec("default.png"));
        } else {
          mat->texture(render::TextureSpec("odd_space_2.png"));
        }
      }});
    }
    core::Core::Instance->get<job::Manager>()->dispatch(castJobs)->wait();
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