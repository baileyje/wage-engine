#pragma once

#include <vector>

#include "core/service.h"
#include "ecs/manager.h"

#include "messaging/messaging.h"

namespace wage {
  namespace physics {

    /**
     * System providing physics simulation to the engine. This requires a concrete implemantion to be provided. The base system will 
     * be responsible for ensure any entities that are added to the game are properly considered for physics simulation. 
     */
    class Physics : public core::Service, messaging::MessageListener<ecs::AddEntityMessage>, messaging::MessageListener<ecs::DestroyEntityMessage> {

    public:
      Physics() : Service("Physics") {}

      ~Physics() {}

      /**
       * Start the system. At the base level this will register this system to receive updates as entities are added and removed form the
       * the game to add them to the physics simulation.
       */
      void start() {
        core::Core::Instance->get<messaging::Messaging>()->listen<ecs::AddEntityMessage>(this);
        core::Core::Instance->get<messaging::Messaging>()->listen<ecs::DestroyEntityMessage>(this);
      }

      /**
       * Pure virtual method that must be implemanted to add an entity into the physics simulation.
       */
      virtual void add(ecs::Entity entity) = 0;

      /**
       * Pure virtual method that must be implemanted to remove an entity into the physics simulation.
       */
      virtual void remove(ecs::Entity entity) = 0;

      /**
       * Message listener triggered by entities being added to the game.
       */
      inline bool on(const ecs::AddEntityMessage& message) {
        auto entity = message.entity();
        if (
            (entity.has(RigidBodyComponent)) || (entity.has(ColliderComponent))) {
          add(entity);
        }
        return false;
      }

      /**
       * Message listener triggered by entities being removed from the game.
       */
      inline bool on(const ecs::DestroyEntityMessage& message) {
        remove(message.entity());
        return false;
      }
    };

  }
}