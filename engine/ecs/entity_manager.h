#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "core/core.h"
#include "core/service.h"
#include "messaging/messaging.h"

#include "ecs/registry.h"
#include "ecs/entity.h"
#include "ecs/view.h"

namespace wage { namespace ecs {

  class EntityMessage {

  public:
    EntityMessage(ecs::Entity entity) : _entity(entity) {}

    virtual ~EntityMessage() {}

    inline ecs::Entity entity() const {
      return _entity;
    }

  private:
    ecs::Entity _entity;
  };

  class AddEntityMessage : public EntityMessage {

  public:
    AddEntityMessage(ecs::Entity entity) : EntityMessage(entity) {}
  };

  class DestroyEntityMessage : public EntityMessage {

  public:
    DestroyEntityMessage(ecs::Entity entity) : EntityMessage(entity) {}
  };

  class EntityManager : public core::Service, public ecs::Entity::Source {

  public:
    EntityManager() : Service("EntityManager") {}

    virtual ~EntityManager() {}

    void start() override {
      core::Core::Instance->onUpdate([&](const core::Frame& frame) {
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
      auto messaging = core::Core::Instance->get<messaging::Messaging>();
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

    // inline ecs::Entity create() {
    //   auto entity = _ecs.create();
    //   adds.push_back(entity);
    //   return entity;
    // }

    inline ecs::Entity create() {
      auto id = _registry.create();
      ecs::Entity entity(id, &_registry);
      adds.push_back(entity);
      return entity;
    }

    inline ecs::Entity get(ecs::EntityId id) override {
      auto versionedId = _registry.getEntity(id);
      return { versionedId, &_registry };
    }

    template <typename Component>
    ecs::View<Component> with() {
      return { this, _registry.pool<Component>() };
    }

    inline void destroy(ecs::Entity entity) {
      destroyRequests.push_back(entity);
    }

    inline ecs::Registry&registry() {
      return _registry;
    }

  private:

    ecs::Registry _registry;

    std::vector<ecs::Entity> adds;

    std::vector<ecs::Entity> destroyRequests;

    std::vector<ecs::Entity> destroys;
  };

} }
