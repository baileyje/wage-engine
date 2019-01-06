#include "engine/entity/entity.h"

#include "engine/memory/allocator.h"

#include "engine/entity/component.h"
#include "engine/entity/context.h"
#include "engine/entity/component/context.h"
#include "engine/entity/reference.h"
#include "engine/entity/component/func_component.h"

namespace wage {

  // #define LIFECYCLE_FUNC(Name) \
  // void Entity::Name(EntityContext* context) {  \
  //   EntityComponentContext componentContext(this, context); \
  //   for (auto component : dynamicComponents) { \
  //     component->Name(&componentContext); \
  //   } \
  // } \
  // 


  EntityId Entity::CurrentId = 1; // 0 = invalid Id

  Entity::~Entity() {}

  // LIFECYCLE_FUNC(start)
  // LIFECYCLE_FUNC(update)
  // LIFECYCLE_FUNC(fixedUpdate)
  // LIFECYCLE_FUNC(stop)

  // Entity* Entity::onUpdate(ComponentCallback func) {
  //   return create<FunctionComponent>(func);;
  // }

}