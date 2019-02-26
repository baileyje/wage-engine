#ifndef ECS_REGISTRY_H
#define ECS_REGISTRY_H

#include <unordered_map>
#include <functional>
#include <typeinfo>
#include <typeindex>

#include "memory/object_pool.h"

#include "ecs/component_pool.h"
#include "ecs/view.h"

namespace wage {

  typedef uint32_t EntityId;

  class Registry;

  class Entity {
  
  public:

    Entity() : Entity(nullptr, Reference<EntityId, EntityId>()) {
    }

    Entity(Registry* registry, Reference<EntityId, EntityId> reference) : wrapped(reference), registry(registry) {
    }

    Entity(const Entity& src) {
      wrapped = std::move(src.wrapped);
      registry = std::move(src.registry);
    }

    Entity(Entity&& src) {
      wrapped = std::move(src.wrapped);
      registry = std::move(src.registry);
    }

    inline EntityId id() {
      return wrapped.index();
    }

    inline bool valid() {
      return registry != nullptr && wrapped.valid();
    }
    
    // EntityId* operator->() {
    //   assert(wrapped.valid());
    //   return wrapped.operator->();
    // }

    // EntityId& operator*() {
    //   assert(wrapped.valid());
    //   return wrapped.operator*();
    // }

    bool operator==(const Entity& other) const {
      return wrapped.operator==(other.wrapped);
    }
    
    bool operator!=(const Entity& other) const {
      return wrapped.operator!=(other.wrapped);
    }

    void destroy() {
      assert(wrapped.valid());
      wrapped.destroy();
    }

    template <typename C, typename... Args>
    Reference<C, ComponentId> assign(Args... args);

    template <typename C>
    bool has();

    template <typename C>
    Reference<C, ComponentId> get();


  private:

    Reference<EntityId, EntityId> wrapped;

    Registry* registry;

  };

  class Registry {
  
  public:

    inline Entity create() {
      return { this, entities.create() };
    }

    inline Entity get(EntityId id) {
      return { this, entities.reference(id) };
    }

    template <typename C, typename... Args>
    inline Reference<C, ComponentId> assign(EntityId entityId, Args... args) {
      return pool<C>()->assign(entityId, args...);
    }

    template <typename C>
    inline bool has(EntityId entityId) {
      return pool<C>()->has(entityId);
    }

    template <typename C>
    inline Reference<C, ComponentId> get(EntityId entityId) {
      return pool<C>()->get(entityId);
    }

    template <typename C>
    inline View<Entity, C> with() {
      return { pool<C>(), [&](EntityId id) {
        return get(id);
      } };
    }
    
  private:

    template <typename C>
    inline ComponentPool<C>* pool() {
      auto& typeId = typeid(C);
      if (componentPools.find(typeId) == componentPools.end()) {
        componentPools[typeId] = new ComponentPool<C>();
      }
      return static_cast<ComponentPool<C>*>(componentPools[typeId]);
    }

    ObjectPool<EntityId, EntityId> entities;

    std::unordered_map<std::type_index, BaseComponentPool*> componentPools;

  };

  template <typename C, typename... Args>
  Reference<C, ComponentId> Entity::assign(Args... args) {
    return registry->assign<C>(id(), args...);
  }

  template <typename C>
  bool Entity::has() {
    return registry->has<C>(id());
  }
  template <typename C>
  Reference<C, ComponentId> Entity::get() {
    return registry->get<C>(id());
  }

}

#endif // ECS_REGISTRY_H