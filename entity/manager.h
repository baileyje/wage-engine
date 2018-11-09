#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <unordered_map>
#include <string>
// #include <algorithm>

#include "core/system.h"

#include "entity/entity.h"

#include "memory/object_pool.h"

typedef ObjectPool<Entity>::Reference EntityReference;

typedef std::vector<EntityReference> EntityList;

typedef ObjectPool<Entity>::Iterator EntityIterator;


class EntityManager: public System {

public:

  EntityManager() : System("EntityManager") {}

  virtual ~EntityManager() {}

  inline EntityReference create() {
    EntityReference ref = pool.create();
    ref->setId(Entity::nextId());
    byId[ref->getId()] = ref;
    return ref;
  }

  inline void destroy(EntityReference reference) {
    byId.erase(reference->getId());
    reference.free();
  }

  inline EntityReference get(EntityId id) {
    return byId[id];
  }

  EntityList with(std::string componentName);

  inline EntityIterator begin() {
    return pool.begin();
  }

  inline EntityIterator end() {
    return pool.end();
  }

  inline void debug() {
    pool.debug();    
  }

private:

  std::unordered_map<EntityId, EntityReference> byId;

  ObjectPool<Entity> pool;

};

#endif // ENTITY_MANAGER_H