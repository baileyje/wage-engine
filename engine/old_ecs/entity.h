#pragma once

#include <vector>
#include "util/sparse_set.h"
#include "ecs/common.h"
#include "ecs/registry.h"

namespace wage { namespace ecs {

  class Entity {
  public:

    class Source {
    public:
      virtual Entity get(EntityId id) = 0;
    };

    Entity(): _id(INVALID_ENTITY_ID, 0) {}

    Entity(VersionedEntityId id, Registry* registry): _id(id), registry(registry) {}

    EntityId id() const {
      return _id.id();
    }

    bool operator==(const Entity& other) const {
      return registry == other.registry && _id == other._id;
    }

    bool operator!=(const Entity& other) const {
      return !this->operator==(other);
    }

    bool valid() const {
      return registry->valid(_id);
    }

    inline void destroy() {
      registry->destroy(_id);
    }

    template <typename Component>
    inline memory::Reference<Component, ComponentId> get() {
      return registry->get<Component>(_id);
    }

    template <typename Component>
    inline bool has() {
      return registry->has<Component>(_id);
    }

    template <typename Component, typename... Args>
    inline memory::Reference<Component, ComponentId> assign(Args... args) {
      return registry->assign<Component>(_id, args...);
    }

  private:

    VersionedEntityId _id;

    Registry* registry;

  };

}
  typedef ecs::Entity Entity;
}
