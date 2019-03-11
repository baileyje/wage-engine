#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <unordered_map>
#include <string>

#include "core/core.h"
#include "core/service.h"
#include "memory/object_pool.h"
#include "messaging/messaging.h"

#include "ecs/registry.h"

namespace wage {

  typedef ObjectPool<Entity, EntityId>::Iterator EntityIterator;

  class EntityMessage {

  public:
    EntityMessage(Entity entity) : _entity(entity) {}

    virtual ~EntityMessage() {}

    inline Entity entity() const {
      return _entity;
    }

  private:
    Entity _entity;
  };

  class AddEntityMessage : public EntityMessage {

  public:
    AddEntityMessage(Entity entity) : EntityMessage(entity) {}
  };

  class DestroyEntityMessage : public EntityMessage {

  public:
    DestroyEntityMessage(Entity entity) : EntityMessage(entity) {}
  };

  class EntityManager : public Service {

  public:
    EntityManager() : Service("EntityManager") {}

    virtual ~EntityManager() {}

    void start() override {
      Core::Instance->onUpdate([&](const Frame& frame) {
        update();
      });
    }

    /*
      1.  Send Add Messages
      2.  Destroy entities
      4.  Send requested destroy messages
      4.  Queue destroys
    */
    void update() {
      auto messaging = Core::Instance->get<Messaging>();
      if (!messaging) {
        return;
      }
      for (auto entity : adds) {
        AddEntityMessage message(entity);
        messaging->send(message);
      }
      adds.clear();

      for (auto entity : destroys) {
        entity.destroy();
      }
      destroys.clear();

      for (auto entity : destroyRequests) {
        DestroyEntityMessage message(entity);
        messaging->send(message);
        destroys.push_back(entity);
      }
      destroyRequests.clear();
    }

    inline Entity create() {
      auto ref = _registry.create();
      adds.push_back(ref);
      return ref;
    }

    inline void destroy(Entity reference) {
      destroyRequests.push_back(reference);
    }

    inline Entity get(EntityId id) {
      return _registry.get(id);
    }

    inline Registry* registry() {
      return &_registry;
    }

  private:
    Registry _registry;

    std::vector<Entity> adds;

    std::vector<Entity> destroyRequests;

    std::vector<Entity> destroys;
  };
}

#endif // ENTITY_MANAGER_H