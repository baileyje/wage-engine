#include "engine/engine.h"


void Engine::start(Context* context) {
  // TODO: Expose engine to JSRT
}

void Engine::update(Context* context) {
  for (auto entity : entities) {
    for (auto component : *entity->getComponents()) {
      component->update(context);
    }
  }
}

void Engine::fixedUpdate(Context* context) {
  for (auto entity : entities) {
    for (auto component : *entity->getComponents()) {
      component->fixedUpdate(context);
    }
  }
}

void Engine::add(Entity* entity) {
  entities.push_back(entity);
}
