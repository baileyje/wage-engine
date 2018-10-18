#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "entity/transform.h"

class Component;

class Entity {

public:
  
  Entity();

  Entity(Transform transform);
  
  virtual ~Entity();

  long getId() { return id; }
  
  Transform* getTransform() { return &transform; }

  void setTransform(Transform transform) { this->transform = transform; }

  void add(Component* component) { components.push_back(component); }

  std::vector<Component*>* getComponents() { return &components; }
  
private:
  
  long id;

  Transform transform;

  std::vector<Component*> components;
  
  static long CurrentId;

};

#endif //ENTITY_H