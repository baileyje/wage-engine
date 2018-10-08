#include "entity/entity.h"

long Entity::CurrentId = 0;

// TODO: DITCH JANKY ID GEN
Entity::Entity() : id(Entity::CurrentId++) {
}

Entity::~Entity() {}

void Entity::add(Component* component) {
  components.push_back(component);
}