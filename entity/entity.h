#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "entity/transform.h"
#include "entity/component.h"

class Entity {

public:
  
  Entity();

  virtual ~Entity();

  long getId() { return id; }
  
  Transform* getTransform() { return &transform; }

  void add(Component* component);

  std::vector<Component*>* getComponents() { return &components; }
  
private:
  
  long id;

  Transform transform;

  std::vector<Component*> components;
  
  static long CurrentId;

};

#endif //ENTITY_H