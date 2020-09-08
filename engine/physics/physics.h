#pragma once

#include <vector>
#include <map>

#include "core/service.h"
#include "ecs/manager.h"

#include "messaging/messaging.h"
#include "physics/collision.h"
#include "physics/ray_hit.h"

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
        core::Core::Instance->onFixedUpdate([&](const core::Frame& frame) {
          fixedUpdate(frame);
        });
      }

      virtual void fixedUpdate(const core::Frame& frame) {
        _collisions.clear();
        _collisionsById.clear();
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
        if ((entity.has(RigidBodyComponent)) || (entity.has(ColliderComponent))) {
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

      /**
       * Get all the collisions encountered within the physics world in the last fixed update.
       */
      inline std::vector<Collision>& collisions() {
        return _collisions;
      }

      /**
       * Get all the collision involving a single entity in the last fixed update.
       */
      inline std::vector<Collision>& collisionsFor(ecs::Entity entity) {
        return _collisionsById[entity.id().id()];
      }

      /**
       * Cast a ray between two points in the world and return any entities that were in the ray path.
       */
      virtual std::vector<RayHit> castRay(math::Vector3 from, math::Vector3 to) = 0;

      virtual void reset() {
        _collisions.clear();
        _collisionsById.clear();
      }

    protected:
      /**
       * Primary entry point for a physics implementation to make collisions available to the rest of the engine.
       */
      inline void addCollision(Collision collision) {
        _collisions.push_back(collision);
        _collisionsById[collision.entity().id().id()].push_back(collision);
      }

    private:
      std::vector<Collision> _collisions;
      std::unordered_map<ecs::EntityId, std::vector<Collision>> _collisionsById;
    };

  }
}