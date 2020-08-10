#pragma once

#include <typeinfo>
#include <typeindex>
#include <unordered_map>

#include "ecs/common.h"
#include "ecs/id_pool.h"
#include "ecs/component_pool.h"

namespace wage { namespace ecs {

  class Registry {
  public:

    inline VersionedEntityId create() {
      return entityIdPool.create();
    }

    inline void destroy(VersionedEntityId id) {
      return entityIdPool.destroy(id);
    }

    inline bool valid(VersionedEntityId id) {
      return entityIdPool.valid(id);
    }

    template <typename Component, typename... Args>
    inline memory::Reference<Component, ComponentId> assign(VersionedEntityId entityId, Args... args) {
      return pool<Component>()->assign(entityId.id(), args...);
    }

    template <typename Component>
    inline bool has(VersionedEntityId entityId) {
      return pool<Component>()->has(entityId.id());
    }

    template <typename Component>
    inline memory::Reference<Component, ComponentId> get(VersionedEntityId entityId) {
      return pool<Component>()->get(entityId.id());
    }

    template <typename Component>
    inline ComponentPool<Component>* pool() {
      auto& typeId = typeid(Component);
      if (componentPools.find(typeId) == componentPools.end()) {
        auto pool = new ComponentPool<Component>();
        componentPools[typeId] = pool;
      }
      return static_cast<ComponentPool<Component>*>(componentPools[typeId]);
    }

    virtual VersionedEntityId getEntity(EntityId id) {
      return entityIdPool.get(id);
    }

  private:

    IdPool<EntityId, EntityVersion> entityIdPool;

    std::unordered_map<std::type_index, BaseComponentPool*> componentPools;
  };

} }
