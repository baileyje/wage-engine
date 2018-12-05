#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <unordered_map>
#include <string>


#include "core/system.h"
#include "core/system/context.h"
#include "entity/entity.h"
#include "memory/object_pool.h"
#include "messaging/messaging.h"

typedef ObjectPool<Entity>::Reference EntityReference;

typedef std::vector<EntityReference> EntityList;

typedef ObjectPool<Entity>::Iterator EntityIterator;

class EntityMessage {

public:

  EntityMessage(EntityReference entity) : entity(entity) {}
  
  virtual ~EntityMessage() {}

  inline EntityReference getEntity() {
    return entity;
  }

  private:

    EntityReference entity;
};

class AddEntityMessage : public EntityMessage {  

public:

  AddEntityMessage(EntityReference entity) : EntityMessage(entity) {}
};

class DestroyEntityMessage : public EntityMessage {  

public:

  DestroyEntityMessage(EntityReference entity) : EntityMessage(entity) {}
};

class EntityManager: public System {

public:

  EntityManager() : System("EntityManager") {}

  virtual ~EntityManager() {}

  void init(SystemContext* context) {
    messaging = context->get<Messaging>();
  }
  
  /*
    1.  Send Add Messages
    2.  Destroy entities
    4.  Send requested destroy messages    
    4.  Queue destroys
  */
  void update(SystemContext* context) {
    if (!messaging) {
      return;
    }
    for (auto entity : adds) {
      AddEntityMessage message(entity);
      messaging->send(message);
    }
    adds.clear();

    for (auto entity : destroys) {
      byId.erase(entity->getId());
      entity.free();
    }
    destroys.clear();

    for (auto entity : destroyRequests) {
      DestroyEntityMessage message(entity);
      messaging->send(message);
      destroys.push_back(entity);
    }
    destroyRequests.clear();
  }

  inline EntityReference create() {
    EntityReference ref = pool.create();
    ref->setId(Entity::nextId());
    byId[ref->getId()] = ref;        
    adds.push_back(ref);
    return ref;
  }

  inline void destroy(EntityReference reference) {
    destroyRequests.push_back(reference);    
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

  Messaging* messaging;

  std::vector<EntityReference> adds;
  
  std::vector<EntityReference> destroyRequests;
  
  std::vector<EntityReference> destroys;

};

#endif // ENTITY_MANAGER_H