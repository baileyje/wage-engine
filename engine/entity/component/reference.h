#ifndef ENTITY_COMPONENT_REFERENCE_H
#define ENTITY_COMPONENT_REFERENCE_H

#include "memory/reference.h"
#include "entity/component.h"
#include "entity/component/reference.h"
#include "entity/entity.h"

namespace wage {

  template <typename T>
  class ComponentReference {
  
  public: 

    ComponentReference() : _id(0), _entity(Reference<Entity>()), _component(nullptr) {}

    ComponentReference(ComponentId id, Reference<Entity> entity, T* component) : _id(id), _entity(entity), _component(component) {
    }

    static ComponentReference<T> NotFound;

    T* operator->() {
      return component();
    }

    const T* operator->() const {
      return const_cast<ComponentReference*>(this)->operator->();
    }

    T& operator*() {
      return *component();
    }

    const T& operator*() const {
      return const_cast<ComponentReference<T>*>(this)->operator*();
    }

    inline ComponentId id() {
      return _id;
    }

    inline Reference<Entity> entity() {
      return _entity;
    }

    inline T* component() {
      return _component;
    }

    inline bool valid() {
      return _component != nullptr;
    }

  private:
    
    ComponentId _id;

    Reference<Entity> _entity;
    
    T* _component;
   
  };

}

#endif //ENTITY_COMPONENT_REFERENCE_H