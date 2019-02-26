#ifndef ECS_VIEW_H
#define ECS_VIEW_H

#include <functional>

#include "ecs/component_pool.h"

namespace wage {

  template <typename Entity, typename C>
  class View {

  public:

    typedef std::function<Entity(EntityId)> Getter;

    class Iterator {
      
      typedef typename ComponentPool<C>::EntityComponentPair Pair;

      typedef typename ObjectPool<Pair, ComponentId>::Iterator WrappedIterator;

    public:
      
      Iterator(View<Entity,C>* view, WrappedIterator wrapped) : view(view), wrapped(wrapped) {
      }

      ~Iterator() {}

      bool operator==(const Iterator& other) const {
        return view->pool == other.view->pool && wrapped == other.wrapped;
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
        return view->getter((*wrapped)->entityId());
      }

      Entity operator->() { 
        return view->getter((*wrapped)->entityId());
      }

    private:
      
      View<Entity, C>* view;

      WrappedIterator wrapped;

    };

    View(ComponentPool<C>* pool, Getter getter) : pool(pool), getter(getter) {}

    Iterator begin() {
      return Iterator(this, pool->begin());
    }

    Iterator end() {
      return Iterator(this, pool->end());
    }

  private:

    ComponentPool<C>* pool;

    Getter getter;

  };

}

#endif // ECS_VIEW_H
