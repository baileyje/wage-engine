#include "engine/engine.h"

#include "entity/entity_context.h"
#include "entity/component.h"


void Engine::start(Context* context) {
  // TODO: Expose engine to JSRT
}

void Engine::update(Context* context) {
  for (auto entity : *context->getEntities()) {
    EntityContext entityContext(entity, context->time, context->deltaTime);
    for (auto component : *entity->getComponents()) {
      component->update(&entityContext);
    }
  }
}

void Engine::fixedUpdate(Context* context) {
  for (auto entity : *context->getEntities()) {
    EntityContext entityContext(entity, context->time, context->deltaTime);
    for (auto component : *entity->getComponents()) {
      component->fixedUpdate(&entityContext);
    }
  }
}
