#ifndef ENTITY_REFERENCE_H
#define ENTITY_REFERENCE_H

#include "engine/memory/reference.h"
#include "engine/entity/entity.h"
#include "engine/entity/component/manager.h"

namespace wage {

  class ComponentContext;

  typedef std::function<void(ComponentContext* context)> ComponentCallback;

  class EntityReference {
  
  public:

    EntityReference() : EntityReference(nullptr, Reference<Entity>()) {
    }

    EntityReference(ComponentManager* componentManager, Reference<Entity> reference) : componentManager(componentManager), wrapped(reference) {
    }

    inline bool isValid() {
      return wrapped.isValid();
    }

    template <typename T, typename... Args>
    inline EntityReference create(Args... args) {
      componentManager->create<T>(wrapped, args...);
      return *this;
    }

    template <typename T, typename I, typename... Args>
    inline EntityReference create(Args... args) {
      componentManager->create<T, I>(wrapped, args...);
      return *this;
    }

    template <class T>
    inline EntityReference add(T* component) { 
      componentManager->add(wrapped, component);
      return *this;
    }

    EntityReference onUpdate(ComponentCallback func);

    template <typename T>
    inline ComponentReference<T> get() { 
      return componentManager->get<T>(wrapped);
    }

    Entity* operator->() {
      assert(wrapped.isValid());
      return wrapped.operator->();
    }

    bool operator==(const EntityReference& other) const {
      return wrapped.operator==(other.wrapped);
    }
    
    bool operator!=(const EntityReference& other) const {
      return wrapped.operator!=(other.wrapped);
    }

    void free() {
      assert(wrapped.isValid());
      wrapped.free();
    }

    // const T* operator->() const {
    //   return const_cast<Reference*>(this)->operator->();
    // }

    Entity& operator*() {
      assert(wrapped.isValid());
      return wrapped.operator*();
    }

    // const T& operator*() const {
    //   return const_cast<Reference<T>*>(this)->operator*();
    // }

    // TODO: This is garbage.
    Reference<Entity> getWrapped() {
      return wrapped;
    }

  private:

    ComponentManager* componentManager;

    Reference<Entity> wrapped;
  };

}

#endif //ENTITY_REFERENCE_H
