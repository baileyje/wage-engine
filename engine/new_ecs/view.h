#pragma once

#include "new_ecs/registry.h"
#include "new_ecs/entity.h"

// Give this some action with iterating actual entiies.

namespace wage { namespace ecs {

  template <typename C>
  class View {
  public:

    View(Entity::Source* source, ComponentPool<C>* pool): source(source), _pool(pool) {
    }

    class Iterator {

      typedef typename ComponentPool<C>::Item Item;

      typedef typename ObjectPool<Item, ComponentId>::Iterator WrappedIterator;

    public:
      Iterator(Entity::Source* source, WrappedIterator wrapped) : source(source), wrapped(wrapped) {
      }

      ~Iterator() {}

      bool operator==(const Iterator& other) const {
        return source == other.source && wrapped == other.wrapped;
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

      Entity operator*() {
        return source->get((*wrapped)->entityId());
      }

      EntityId operator->() {
        return source->get((*wrapped)->entityId());
      }

    private:
      Entity::Source* source;

      WrappedIterator wrapped;
    };

    Iterator begin() {
      return Iterator(source, _pool->begin());
    }

    Iterator end() {
      return Iterator(source, _pool->end());
    }

  private:

    Entity::Source* source;

    ComponentPool<C>* _pool;

  };

} }
