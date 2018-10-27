#ifndef ENTITY_STORE_H
#define ENTITY_STORE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

#include "entity/entity.h"

typedef std::vector<Entity*> EntityList;
typedef std::vector<Entity*>::iterator EntityIterator;

class EntityStore {

public:

  EntityStore() {}

  ~EntityStore() {}

  inline void add(Entity* entity) {
    entities.push_back(entity);
    byId[entity->getId()] = entity;
  }

  inline Entity* get(long id) {
    return entities[id];
  }

  EntityList with(std::string componentName) {
    std::vector<Entity*> found;
    for (auto entity : entities) {
      if (entity->getComponents()->get(componentName)) {
        found.push_back(entity);
      }
    }
    return found;
  }

  inline EntityIterator begin() {
    return entities.begin();
  }

  inline EntityIterator end() {
    return entities.end();
  }  

private:

  std::vector<Entity*> entities;

  std::unordered_map<long, Entity*> byId;

};

#endif // ENTITY_STORE_H