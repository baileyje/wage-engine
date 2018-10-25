#ifndef ENTITY_STORE_H
#define ENTITY_STORE_H

#include <vector>
#include <string>

#include "entity/entity.h"

class EntityStore {

public:

  EntityStore() {}

  ~EntityStore() {}

  inline void add(Entity* entity) {
    entities.push_back(entity);
  }

  std::vector<Entity*> getEntitiesWith(std::string componentName) {
    std::vector<Entity*> found;
    for (auto entity : entities) {
      if (entity->getComponent(componentName)) {
        found.push_back(entity);
      }
    }
    return found;
  }

private:

  std::vector<Entity*> entities;

};

#endif // ENTITY_STORE_H