#include "entity/entity.h"

#include "entity/component.h"

long Entity::CurrentId = 0;

// TODO: DITCH JANKY ID GEN
Entity::Entity() : id(Entity::CurrentId++) {
}

Entity::~Entity() {}
