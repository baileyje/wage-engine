#include "entity/entity.h"

#include "memory/allocator.h"

#include "entity/component.h"
#include "entity/context.h"
#include "entity/component/context.h"
#include "entity/reference.h"
#include "entity/component/func_component.h"

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