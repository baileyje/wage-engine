#pragma once

#include "ecs/common.h"
#include "ecs/component_pool.h"

namespace wage {
  namespace ecs {

    class Registry;

    class Entity {
      friend class Registry;

    public:
      static Entity Invalid;

      /**
       * Get the ID.
       */
      inline VersionedId<EntityId> id() {
        return _id;
      }

      /**
       * Determine if this entity reference is still valid.
       */
      bool valid();

      bool operator==(const Entity& other) const {
        return registry == other.registry && _id == other._id;
      }

      bool operator!=(const Entity& other) const {
        return !this->operator==(other);
      }
      /**
       * Allocate the data associated with this component for a given entity id.
       */
      memory::Pool::Item allocateComponent(ComponentType componentType);

      /**
       * Assign a component to an entity by performing a placement new at the newly allocated item with provided args.
       */
      template <class Component, typename... Args>
      inline Component* assign(ComponentType componentType, Args... args) {
        auto storage = allocateComponent(componentType);
        return new (storage) Component(args...);
      }

      /**
       * Just slam data into a newly allocated item. Maybe this isn't great...
       */
      template <typename Component, typename... Args>
      inline Component* set(ComponentType componentType, Component component) {
        auto storage = allocateComponent(componentType);
        memcpy(storage, &component, sizeof(Component));
        return static_cast<Component*>(storage);
      }

      /**
       * Get the component for a specific entity.
       */
      template <typename Component>
      inline Component* get(ComponentType componentType) {
        return static_cast<Component*>(getStorage(componentType));
      }

      /**
       * Check if an entity has a specific component type.
       */
      bool has(ComponentType componentType);

      /**
       *  Get a component instance by component id.
       */
      memory::Pool::Item getStorage(ComponentType componentType);

    private:
      Entity(Registry* registry, VersionedId<EntityId> id) : registry(registry), _id(id) {}

      Registry* registry;

      VersionedId<EntityId> _id;
    };
  }
}