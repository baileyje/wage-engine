#pragma once

#include "new_ecs/entity.h"
#include "new_ecs/component_pool.h"
#include "new_ecs/id_pool.h"
#include "new_ecs/view.h"
#include "memory/indexed_storage.h"

namespace wage {
  namespace ecs {

    /**
     * Store the component pools for maintaining entity state. Maintain any active state related to entities.
     */
    class Registry {

      friend class Entity;

    public:
      Registry() {
      }

      /**
       * Create a new entity with a free id.
       */
      inline Entity createEntity() {
        return {this, entityIds.create()};
      }

      /**
       * Get an entity reference for a specific id..
       */
      inline Entity getEntity(EntityId id) {
        return {this, entityIds.get(id)};
      }

      /**
       * Get an entity reference for a specific id..
       */
      inline bool valid(VersionedId<EntityId> id) {
        return entityIds.valid(id);
      }

      /**
       * Register a new component type with unique id and storage size.
       */
      inline void registerComponent(ComponentType type, size_t componentSize) {
        // TODO: Assert we didn't go over max component id
        knownComponents.push_back(type);
        auto storage = pools[type];
        new (storage) ComponentPool(componentSize);
      }

      /**
       * Allocate the data associated with this component for a given entity id.
       */
      inline ComponentPool::Item& allocateComponent(VersionedId<EntityId> entityId, ComponentType componentType) {
        return poolFor(componentType)->allocate(entityId.id());
      }

      /**
       * Get the component storage for a specific entity and component id.
       */
      inline ComponentPool::Item& getComponent(VersionedId<EntityId> entityId, ComponentType componentType) {
        return poolFor(componentType)->get(entityId.id());
      }

      /**
       * Determine if an entity has a specific component by type.
       */
      inline bool hasComponent(VersionedId<EntityId> entityId, ComponentType componentType) {
        return poolFor(componentType)->has(entityId.id());
      }

      /**
       *  Destroy the entity at the provided id and free it for recycling.
       */
      inline void destroy(VersionedId<EntityId> entityId) {
        for (auto componentType : knownComponents) {
          poolFor(componentType)->destroy(entityId.id());
        }
        entityIds.destroy(entityId);
      }

      View<Registry> view(std::vector<ComponentType> types) {
        // TODO: Assert at least one type.
        ComponentPool* basePool = nullptr;
        size_t minSize = MAX_ENTITY_ID;
        for (auto type : types) {
          auto pool = poolFor(type);
          if (pool->currentSize() < minSize) {
            minSize = pool->currentSize();
            basePool = pool;
          }
        }
        return {this, basePool, types};
      }

      /**
       * Determine the number of components of a give type in the registry.
       */
      inline size_t componentCount(ComponentType componentType) {
        return poolFor(componentType)->currentSize();
      }

    private:

      inline ComponentPool* poolFor(ComponentType componentType) {
        return pools[componentType];
      }

      std::vector<ComponentType> knownComponents;

      memory::ChunkyIndexedStorage<ComponentPool> pools;

      IdPool<EntityId> entityIds;

    };

  }
}