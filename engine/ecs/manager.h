#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <functional>

#include "core/core.h"
#include "core/service.h"
#include "messaging/messaging.h"

#include "ecs/registry.h"
#include "ecs/entity.h"
#include "ecs/view.h"
#include "ecs/messages.h"

namespace wage {
  namespace ecs {

    class EntityManager : public core::Service {

    public:
      EntityManager() : Service("EntityManager") {}

      virtual ~EntityManager() {}

      // void start() override {
      //   core::Core::Instance->onUpdate([&](const core::Frame& frame) {
      //     update();
      //   });
      // }

      /*
        1.  Send Add Messages
        2.  Destroy entities
        4.  Send requested destroy messages
        4.  Queue destroys
      */
      void update() {
        auto messaging = core::Core::Instance->get<messaging::Messaging>();
        if (messaging) {
          for (auto entity : adds) {
            AddEntityMessage message(entity);
            messaging->send(message);
          }
        }
        adds.clear();

        for (auto entity : destroys) {
          printf("Fully destroying\n");
          _registry.destroy(entity.id());
        }
        destroys.clear();
        for (auto entity : destroyRequests) {
          printf("Letting others know of pending destroy\n");
          if (messaging) {
            DestroyEntityMessage message(entity);
            messaging->send(message);
          }
          destroys.push_back(entity);
        }
        destroyRequests.clear();
      }

      /**
       * Create a new entity.
       */
      inline Entity create() {
        auto entity = _registry.createEntity();
        adds.push_back(entity);
        return entity;
      }

      /**
       * Get the current entity for a given entity id.
       */
      inline Entity get(EntityId id) {
        return _registry.getEntity(id);
      }

      /**
       * Register a new component type with entity system.
       */
      inline void registerComponent(ComponentType type, size_t componentSize) {
        _registry.registerComponent(type, componentSize);
      }

      /**
       * Return an entity view that can iterate all entities that contain all the provided component types.
       */
      View<Registry> with(std::vector<ComponentType> types) {
        return _registry.view(types);
      }

      /**
       * Destroy an entity at the next safe opportunity.
       */
      inline void destroy(ecs::Entity entity) {
        printf("Destroy requested\n");
        destroyRequests.push_back(entity);
      }

    private:
      Registry _registry;

      std::vector<Entity> adds;

      std::vector<Entity> destroyRequests;

      std::vector<Entity> destroys;
    };

  }
}
