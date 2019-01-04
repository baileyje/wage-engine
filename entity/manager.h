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
#include "entity/component/manager.h"
#include "entity/component/context.h"
#include "entity/reference.h"
#include "entity/context.h"

namespace wage {

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

  // TODO: Extract out
  class EntityComponentContext : public ComponentContext {

  public:

    EntityComponentContext(EntityReference entity, EntityContext* context) : entity(entity), context(context) {    
    }

    inline Transform& getTransform() {
      return entity->getTransform();
    }

    inline double getTime() const {
      return context->getTime();
    }

    inline double getDeltaTime() const {
      return context->getDeltaTime();
    }

    // inline ComponentMap<Component>* getComponents() {
    //   return entity->getComponents();
    // }

    inline EntityReference getEntity() {
      return entity;
    }
      

  private:

    EntityReference entity;

    EntityContext* context;

  };

  class EntityManager: public System {

  public:

    EntityManager() : System("EntityManager") {}

    virtual ~EntityManager() {}

    void init(SystemContext* context) {
    }

    void start(SystemContext* context) {
      // TODO: Evaluate where this belongs
      EntityContext entityContext(context->getTime(), context->getDeltaTime()); 
      for (auto itr = componentManager_.dynamicBegin(); itr != componentManager_.dynamicEnd(); ++itr) {
        EntityComponentContext componentContext(EntityReference(&componentManager_, (*itr).entity()), &entityContext);
        (*itr)->start(&componentContext);
      }
    }
    
    /*
      1.  Send Add Messages
      2.  Destroy entities
      4.  Send requested destroy messages    
      4.  Queue destroys
    */
    void update(SystemContext* context) {
      auto messaging = context->get<Messaging>();
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
      
      // TODO: Evaluate where this belongs
      EntityContext entityContext(context->getTime(), context->getDeltaTime()); 
      for (auto itr = componentManager_.dynamicBegin(); itr != componentManager_.dynamicEnd(); ++itr) {
        EntityComponentContext componentContext(EntityReference(&componentManager_, (*itr).entity()), &entityContext);
        (*itr)->update(&componentContext);
      }
    }

    void fixedUpdate(SystemContext* context) {
      // TODO: Evaluate where this belongs
      EntityContext entityContext(context->getTime(), context->getDeltaTime()); 
      for (auto itr = componentManager_.dynamicBegin(); itr != componentManager_.dynamicEnd(); ++itr) {
        EntityComponentContext componentContext(EntityReference(&componentManager_, (*itr).entity()), &entityContext);
        (*itr)->fixedUpdate(&componentContext);
      }
    }

    inline EntityReference create() {
      auto poolRef = pool.create();
      EntityReference ref = EntityReference(&componentManager_, poolRef);
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

    // EntityList with(std::string componentName);

    inline EntityIterator begin() {
      return pool.begin();
    }

    inline EntityIterator end() {
      return pool.end();
    }

    inline void debug() {
      pool.debug();    
    }

    inline ComponentManager& componentManager() {
      return componentManager_;
    }

  private:

    std::unordered_map<EntityId, EntityReference> byId;

    ObjectPool<Entity> pool;

    std::vector<EntityReference> adds;
    
    std::vector<EntityReference> destroyRequests;
    
    std::vector<EntityReference> destroys;

    ComponentManager componentManager_;
  };

}

#endif // ENTITY_MANAGER_H