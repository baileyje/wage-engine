#include "engine.h"

#include "core/system/context.h"
#include "entity/context.h"
#include "entity/manager.h"
#include "entity/component.h"

#define LIFECYCLE_FUNC_IMPL(Name) \
void Engine::Name(SystemContext* context) { \
  EntityContext entityContext(context->time(), context->deltaTime()); \
  EntityManager* manager = context->get<EntityManager>(); \
  for (auto& entity : *manager) { \
    entity->Name(&entityContext); \
  } \
} \

namespace wage {

  // LIFECYCLE_FUNC_IMPL(start)
  // LIFECYCLE_FUNC_IMPL(update)
  // LIFECYCLE_FUNC_IMPL(fixedUpdate)
  // LIFECYCLE_FUNC_IMPL(stop)

}
