#include "engine/engine.h"


void Engine::start(Context* context) {
  // TODO: Expose engine to JSRT
}

void Engine::update(Context* context) {
  for (auto entity : entities) {
    for (auto component : *entity->getComponents()) {
      component->update();
    }
  }
}

void Engine::fixedUpdate(Context* context) {
  for (auto entity : entities) {
    for (auto component : *entity->getComponents()) {
      component->fixedUpdate();
    }
  }
}

void Engine::add(Entity* entity) {
  entities.push_back(entity);
}
