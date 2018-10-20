#include "entity/entity.h"

#include "entity/component.h"

long Entity::CurrentId = 0;

// TODO: DITCH JANKY ID GEN
Entity::Entity() : id(Entity::CurrentId++) {
}

Entity::Entity(Transform transform) : id(Entity::CurrentId++), transform(transform) {    
}

Entity::~Entity() {}

// TODO:  Need some kind of efficient store for this.
Component* Entity::getComponent(std::string name) { 
  for (auto comp : components) {
    if (comp->getName() ==  name) {
      return comp;
    }
  }
  return nullptr;
}
