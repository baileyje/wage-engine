#pragma once

#include <vector>

#include "new_ecs/common.h"
#include "memory/pool.h"

namespace wage {
  namespace ecs {

    class ComponentPool {
    public:
      class Item {
        friend class ComponentPool;

      public:
        inline EntityId entityId() {
          return header()->entityId;
        }

        inline void* storage() {
          return static_cast<char*>(_storage) + sizeof(Header);
        }

      private:
        Item() : _storage(nullptr) {}

        Item(memory::Pool::Item storage) : _storage(storage) {
        }

        Item(EntityId entityId, memory::Pool::Item storage) : _storage(storage) {
          header()->entityId = entityId;
        }

        struct Header {
          EntityId entityId;
        };

        Header* header() {
          return reinterpret_cast<Header*>(reinterpret_cast<char*>(_storage));
        }

        memory::Pool::Item _storage;
      };

      class Iterator {
        friend class ComponentPool;
        typedef typename memory::Pool::Iterator WrappedIterator;

      public:
        Iterator(WrappedIterator wrapped) : wrapped(wrapped) {
        }

        ~Iterator() {}

        bool operator==(const Iterator& other) const {
          return wrapped == other.wrapped;
        }

        bool operator!=(const Iterator& other) const {
          return !operator==(other);
        }

        Iterator& operator++() {
          ++wrapped;
          return (*this);
        }

        Iterator operator++(int) {
          Iterator temp = *this;
          ++(*this);
          return temp;
        }

        Iterator& operator--() {
          --wrapped;
          return (*this);
        }

        Iterator operator--(int) {
          Iterator temp = *this;
          --(*this);
          return temp;
        }

        Item operator*() {
          return asItem();
        }

        Item operator->() {
          return asItem();
        }

        Item asItem() {
          return Item(*wrapped);
        }

      private:
        WrappedIterator wrapped;
      };

      ComponentPool() : ComponentPool(0) {}

      ComponentPool(size_t componentSize) : pool(componentSize + sizeof(Item::Header)) {
      }

      /**
       * Allocate the data associated with this component for a given entity id.
       */
      Item& allocate(EntityId entityId) {
        auto storage = pool.allocate();
        sparseRefs[entityId] = Item(entityId, storage);
        return sparseRefs[entityId];
      }

      /**
       * Determine if there is a valid instance of this component type for the entity id provided.
       */
      bool has(EntityId entityId) {
        return sparseRefs[entityId]._storage != nullptr;
      }

      /**
       * Get the component storage associated with the provided entity id.
       */
      Item& get(EntityId entityId) {
        return sparseRefs[entityId];
      }

      /**
       * Destroy the component for a specific entity id.
       */
      void destroy(EntityId entityId) {
        auto item = sparseRefs[entityId];
        if (item._storage != nullptr) {
          pool.deallocate(item._storage);
        }
        sparseRefs[entityId]._storage = nullptr;
      }

      /**
       * Get the begining of the iterator over the active component instances.
       */
      inline Iterator begin() {
        return pool.begin();
      }

      /**
       * Get the end of the iterator over the active component instances.
       */
      inline Iterator end() {
        return pool.end();
      }

      /**
       * Get the current number of components of this type.
       */
      inline size_t currentSize() {
        return pool.currentSize();
      }

    private:
      // Bulk memory is in a compact format - Memory efficiency
      memory::Pool pool;

      // We waist a bunch of pointer space - Lookup efficiency.
      Item sparseRefs[MAX_ENTITY_ID];
    };

  }
}