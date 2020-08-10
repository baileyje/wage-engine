#include "new_ecs/entity.h"
#include "new_ecs/registry.h"

namespace wage {
  namespace ecs {

    void* Entity::allocateComponent(ComponentType componentType) {
      return registry->allocateComponent(_id, componentType).storage();
    }

    void* Entity::getStorage(ComponentType componentType) {
      return registry->getComponent(_id, componentType).storage();
    }

    bool Entity::valid() {
      return registry->valid(_id);
    }

    bool Entity::has(ComponentType componentType) {
      return registry->hasComponent(_id, componentType);
    }
  }
}