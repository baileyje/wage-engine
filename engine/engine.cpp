#include "engine/engine.h"

#include "entity/context.h"
#include "entity/component.h"

#define LIFECYCLE_FUNC_IMPL(Name) \
void Engine::Name(Context* context) { \
  EntityContext entityContext(context->time, context->deltaTime); \
  for (auto& entity : *context->getScene()->getEntities()) { \
    entity->Name(&entityContext); \
  } \
} \

LIFECYCLE_FUNC_IMPL(start)
LIFECYCLE_FUNC_IMPL(update)
LIFECYCLE_FUNC_IMPL(fixedUpdate)
LIFECYCLE_FUNC_IMPL(stop)

