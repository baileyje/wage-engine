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

    ComponentReference() : id_(0), entity_(Reference<Entity>()), component_(nullptr) {}

    ComponentReference(ComponentId id, Reference<Entity> entity, T* component) : id_(id), entity_(entity), component_(component) {
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
      return id_;
    }

    inline Reference<Entity> entity() {
      return entity_;
    }

    inline T* component() {
      return component_;
    }

    inline bool valid() {
      return component_ != nullptr;
    }

  private:
    
    ComponentId id_;

    Reference<Entity> entity_;
    
    T* component_;
   
  };

}

#endif //ENTITY_COMPONENT_REFERENCE_H