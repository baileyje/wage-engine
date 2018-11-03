#ifndef ENTITY_STORE_H
#define ENTITY_STORE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

#include "entity/entity.h"

class EntityReference;

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

  inline Entity* get(EntityId id) {
    return byId[id];
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

  std::unordered_map<EntityId, Entity*> byId;

};

class EntityReference {

public:

  bool empty() const { 
    return store == NULL || store->get(id) == NULL; 
  }

  void clear() {
    store = NULL; 
    id = InvalidEntityId; 
  }

  Entity* get() {
    assert(!empty());
    return store->get(id);
  }

  Entity* operator->() { return get(); }

  Entity& operator*() { return *get(); }

private:

  EntityId id;

  EntityStore* store;
  
};

#endif // ENTITY_STORE_H