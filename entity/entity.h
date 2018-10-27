#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "entity/transform.h"
#include "entity/component_map.h"

class Component;

class Entity {

public:
  
  Entity();

  Entity(Transform transform);

  virtual ~Entity();

  inline long getId() { return id; }
  
  inline Transform* getTransform() { return &transform; }

  inline void setTransform(Transform transform) { this->transform = transform; }

  template <typename T>
  inline Entity* add(T* component) { 
    components.add<T>(component);
    return this;
  }

  template <typename T>
  inline T* get() { 
    return components.get<T>();
  }

  inline ComponentMap* getComponents() { return &components; }
  
private:
  
  long id;

  Transform transform;

  ComponentMap components;
  
  static long CurrentId;

};

#endif //ENTITY_H