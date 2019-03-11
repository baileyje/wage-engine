#ifndef ECS_COMP_POOL
#define ECS_COMP_POOL

#include <vector>
#include <cstdint>

#include "memory/object_pool.h"

#ifndef UINT32_MAX
#define UINT32_MAX ((uint32_t)-1)
#endif

#define INVALID_ID UINT32_MAX

namespace wage {

  typedef uint32_t EntityId;
  typedef uint32_t ComponentId;

  class BaseComponentPool {

  protected:
    BaseComponentPool() {}

    inline void reserve(EntityId toId) {
      sparseIds.resize(toId + 1, INVALID_ID);
    }

    std::vector<ComponentId> sparseIds;
  };

  template <typename C>
  class ComponentPool : public BaseComponentPool, public Reference<C, ComponentId>::Source {

  public:
    class EntityComponentPair {

    public:
      EntityComponentPair() {}

      template <typename... Args>
      EntityComponentPair(EntityId entityId, Args... args) : _entityId(entityId), _component(args...) {}

      inline EntityId& entityId() {
        return _entityId;
      }

      inline C& component() {
        return _component;
      }

    private:
      EntityId _entityId;

      C _component;
    };

    typedef typename ObjectPool<typename ComponentPool<C>::EntityComponentPair, ComponentId>::Iterator Iterator;

    ComponentPool() : BaseComponentPool() {}

    template <typename... Args>
    Reference<C, ComponentId> assign(EntityId entityId, Args... args) {
      reserve(entityId);
      auto ref = pool.create(entityId, args...);
      sparseIds[entityId] = ref.index();
      return Reference<C, ComponentId>(this, ref.index(), ref.version());
    }

    bool has(EntityId entityId) {
      return sparseIds.size() > entityId && sparseIds[entityId] != INVALID_ID;
    }

    Reference<C, ComponentId> get(EntityId entityId) {
      if (sparseIds.size() > entityId && sparseIds[entityId] != INVALID_ID) {
        auto ref = pool.reference(sparseIds[entityId]);
        return Reference<C, ComponentId>(this, ref.index(), ref.version());
      }
      return Reference<C, ComponentId>();
    }

    C* component(ComponentId id) {
      return &pool.reference(id)->component();
    }

    bool valid(Reference<C, ComponentId> ref) {
      return pool.reference(ref.index()).valid();
    }

    void destroy(Reference<C, ComponentId> ref) {
      return pool.reference(ref.index()).destroy();
    }

    C* get(Reference<C, ComponentId> ref) {
      return &pool.reference(ref.index())->component();
    }

    Iterator begin() {
      return pool.begin();
    }

    Iterator end() {
      return pool.end();
    }

  private:
    ObjectPool<EntityComponentPair, ComponentId> pool;
  };
}

#endif // ECS_COMP_POOL