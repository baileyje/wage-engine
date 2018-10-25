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

  long getId() { return id; }
  
  Transform* getTransform() { return &transform; }

  inline void setTransform(Transform transform) { this->transform = transform; }

  template <typename T>
  inline void add(T* component) { 
    components.add<T>(component);
  }

  template <typename T>
  inline T* get() { 
    return components.get<T>();
  }

  inline ComponentMap* getComponents() { return &components; }

  Component* getComponent(std::string name);

  std::vector<Component*> getComponents(std::string name);
  
private:
  
  long id;

  Transform transform;

  ComponentMap components;
  
  static long CurrentId;

};

#endif //ENTITY_H