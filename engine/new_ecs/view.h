#pragma once

#include <vector>

#include "new_ecs/registry.h"
#include "new_ecs/entity.h"

// Give this some action with iterating actual entities.

namespace wage {
  namespace ecs {

    /**
         * Class used to iterate over entities that contain a set of componenent types. The view will only contain entities that have all the components requests.
         */
    template <typename EntitySource>
    class View {
    public:
      /**
             * Create a new view with an entity type and a base component pool to base the iteration on. The components types provided act as a filter for anything provided by the iterator.
             */
      View(EntitySource* source, ComponentPool* basePool, std::vector<ComponentType> componentTypes) : source(source), basePool(basePool), componentTypes(componentTypes) {
      }

      class Iterator {
        friend class View;
        typedef typename ComponentPool::Iterator WrappedIterator;

      public:
        Iterator(View* view, WrappedIterator wrapped) : view(view), wrapped(wrapped) {
        }

        ~Iterator() {}

        bool operator==(const Iterator& other) const {
          return view->source == other.view->source && wrapped == other.wrapped;
        }

        bool operator!=(const Iterator& other) const {
          return !operator==(other);
        }

        Iterator& operator++() {
          do {
            ++wrapped;
          } while (wrapped != view->basePool->end() && !hasAllComponents());
          return (*this);
        }

        Iterator operator++(int) {
          Iterator temp = *this;
          ++(*this);
          return temp;
        }

        Iterator& operator--() {
          do {
            --wrapped;
          } while (wrapped != view->basePool->begin() && hasAllComponents());
          return (*this);
        }

        Iterator operator--(int) {
          Iterator temp = *this;
          --(*this);
          return temp;
        }

        Entity operator*() {
          return currentEntity();
        }

        EntityId operator->() {
          return currentEntity();
        }

      private:
        EntityId currentEntityId() {
          return (*wrapped).entityId();
        }

        Entity currentEntity() {
          return view->source->getEntity(currentEntityId());
        }

        bool hasAllComponents() {
          auto entity = currentEntity();
          for (ComponentType type : view->componentTypes) {
            if (!entity.has(type)) {
              return false;
            }
          }
          return true;
        }

        View* view;

        WrappedIterator wrapped;
      };

      Iterator begin() {
        return Iterator(this, basePool->begin());
      }

      Iterator end() {
        return Iterator(this, basePool->end());
      }

    private:
      EntitySource* source;

      ComponentPool* basePool;

      std::vector<ComponentType> componentTypes;
    };

    // template <typename ComponentTypes...>
    // class TypedView {
    // }

  }
}
