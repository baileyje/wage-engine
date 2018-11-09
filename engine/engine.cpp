#include "engine/engine.h"

#include "core/context.h"
#include "entity/context.h"
#include "entity/manager.h"
#include "entity/component.h"

#define LIFECYCLE_FUNC_IMPL(Name) \
void Engine::Name(Context* context) { \
  EntityContext entityContext(context->getTime(), context->getDeltaTime()); \
  EntityManager* manager = context->get<EntityManager>(); \
  for (auto& entity : *manager) { \
    entity->Name(&entityContext); \
  } \
} \

LIFECYCLE_FUNC_IMPL(start)
LIFECYCLE_FUNC_IMPL(update)
LIFECYCLE_FUNC_IMPL(fixedUpdate)
LIFECYCLE_FUNC_IMPL(stop)

