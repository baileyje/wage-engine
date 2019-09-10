#ifndef ECS_COMPONENT_POOL_H
#define ECS_COMPONENT_POOL_H

#include <vector>

#include "ecs/common.h"
#include "memory/object_pool.h"


namespace wage { namespace ecs {

  class BaseComponentPool {

  protected:
    BaseComponentPool() {}
  };

  template <typename Component>
  class ComponentPool : public BaseComponentPool, public memory::Reference<Component, ComponentId>::Source {

  public:

    class Item;

    typedef memory::Reference<Component, ComponentId> ComponentReference;

    typedef typename memory::ObjectPool<Item, ComponentId>::Iterator Iterator;

    // class View {
    // public:
    //   Iterator begin() {
    //     return Iterator(pool.begin());
    //   }

    //   Iterator end() {
    //     return Iterator(pool->end());
    //   }

    // private:
    //   friend class ComponentPool;
    //   View(Omemory::bjectPool<Item, ComponentId>* pool): pool(pool) {}
    //   ObjectPool<Item, ComponentId>* pool;
    // };

    ComponentPool() : BaseComponentPool() {}

    template <typename... Args>
    ComponentReference assign(EntityId entityId, Args... args) {
      auto ref = pool.create(entityId, args...);
      sparseRefs[entityId] = ref;
      return ComponentReference(this, ref.index(), ref.version());
    }

    bool has(EntityId entityId) {
      return sparseRefs[entityId].valid();
    }

    ComponentReference get(EntityId entityId) {
      if (has(entityId)) {
        auto ref = sparseRefs[entityId];
        return ComponentReference(this, ref.index(), ref.version());
      }
      return ComponentReference();
    }

    Component* component(ComponentId id) {
      return &pool.reference(id)->component();
    }

    bool valid(ComponentReference ref) {
      return pool.reference(ref.index()).valid();
    }

    void destroy(ComponentReference ref) {
      return pool.reference(ref.index()).destroy();
    }

    Component* get(ComponentReference ref) {
      return &pool.reference(ref.index())->component();
    }

    // View view() {
    //   return { &pool };
    // }

    Iterator begin() {
      return Iterator(pool.begin());
    }

    Iterator end() {
      return Iterator(pool.end());
    }

    class Item {
    public:

      inline EntityId& entityId() {
        return _entityId;
      }

      inline Component& component() {
        return _component;
      }

    private:
      friend class ComponentPool;
      friend class memory::ObjectPool<Item, ComponentId>;

      Item() {}

      template <typename... Args>
      Item(EntityId entityId, Args... args) : _entityId(entityId), _component(args...) {}

      friend class ComponentPool;

      EntityId _entityId;

      Component _component;
    };


  private:

    memory::ObjectPool<Item, ComponentId> pool;

    memory::Reference<Item, ComponentId> sparseRefs[MAX_ENTITY_ID];

  };

} }

#endif // ECS_COMPONENT_POOL_H