#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "entity/transform.h"
#include "entity/type_index.h"

class Component;

class Entity {

public:
  
  Entity();

  Entity(Transform transform);

  virtual ~Entity();

  long getId() { return id; }
  
  Transform* getTransform() { return &transform; }

  inline void setTransform(Transform transform) { this->transform = transform; }

  template <typename C>
  inline void add(C* component) { 
    components.push_back(component); 
  }

  inline std::vector<Component*>* getComponents() { return &components; }

  Component* getComponent(std::string name);

  std::vector<Component*> getComponents(std::string name);
  
private:
  
  long id;

  Transform transform;

  std::vector<Component*> components;
  
  static long CurrentId;

};

#endif //ENTITY_H