#pragma once

#include <vector>

#include "ecs/entity.h"
#include "math/vector.h"
#include "physics/contact_point.h"

namespace wage {

  namespace physics {

    /**
     * Simple class holding the data associated with a collision in the physics world.
     */
    class Collision {

    public:
      Collision(ecs::Entity entity, ecs::Entity otherEntity, std::vector<ContactPoint> contacts) : _entity(entity), _otherEntity(otherEntity), _contacts(contacts) {}

      Collision(const Collision& cp) : Collision(cp._entity, cp._otherEntity, cp._contacts) {}

      /**
       * Get the primary entity for this collision.
       */
      inline ecs::Entity entity() {
        return _entity;
      }

      /**
       * Get the other entity involved in this collision.
       */
      inline ecs::Entity otherEntity() {
        return _otherEntity;
      }

      /**
       * Get all the contact points between the two entities.
       */
      inline std::vector<ContactPoint> contacts() {
        return _contacts;
      }

    private:
      ecs::Entity _entity;
      ecs::Entity _otherEntity;
      std::vector<ContactPoint> _contacts;
    };

  }
}
