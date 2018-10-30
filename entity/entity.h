#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "math/transform.h"
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

  inline void add(Entity* child) {
    child->transform.setParent(&transform);
    children.push_back(child);
  }

  inline std::vector<Entity*>* getChildren() {
    return &children;
  }

  inline ComponentMap* getComponents() { return &components; }
  
private:
  
  long id;

  Transform transform;

  ComponentMap components;

  std::vector<Entity*> children;
  
  static long CurrentId;

};

#endif //ENTITY_H