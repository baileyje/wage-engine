#pragma once

#include "engine.h"
#include "enemy.h"
#include "debug.h"

using namespace wage;

void addWall(scene::Scene& scene) {
  int width = 20;
  int height = 20;
  float margin = 0.1;
  float enemyScale = 5;
  float offsetX = -(enemyScale * width / 2);
  float offsetY = -(enemyScale * height / 2);

  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      addEnemy(scene, math::Vector{offsetX, offsetY, 40}, enemyScale);
      offsetY += enemyScale + margin;
    }
    offsetX += enemyScale + margin;
    offsetY = -(enemyScale * height / 2);
  }
}
