#include "ecs/entity.h"
#include "ecs/registry.h"

namespace wage {
  namespace ecs {

    Entity Entity::Invalid = {nullptr, VersionedId<EntityId>(INVALID_ENTITY_ID, 0)};

    void* Entity::allocateComponent(ComponentType componentType) {
      return registry->allocateComponent(_id, componentType).storage();
    }

    void* Entity::getStorage(ComponentType componentType) {
      return registry->getComponent(_id, componentType).storage();
    }

    bool Entity::valid() {
      if (_id.id() == INVALID_ENTITY_ID) return false;
      return registry->valid(_id);
    }

    bool Entity::has(ComponentType componentType) {
      return registry->hasComponent(_id, componentType);
    }
  }
}