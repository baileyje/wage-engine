#pragma once

#include "ecs/entity.h"
#include "math/vector.h"
#include "physics/contact_point.h"

namespace wage {

  namespace physics {

    /**
     * Simple class holding data related to a hit result from a ray cast.
     */
    class RayHit {
    public:
      /**
       * Create a new ray hit.
       */
      RayHit(ecs::Entity entity, math::Vector3 point, math::Vector3 normal) : _entity(entity), _point(point, normal) {}

      /**
       * Create a new ray hit.
       */
      RayHit(ecs::Entity entity, ContactPoint point) : _entity(entity), _point(point) {}

      /**
       * Copy a ray hit.
       */
      RayHit(const RayHit& rh) : RayHit(rh._entity, rh._point) {}

      /**
       * Get the entity the ray hit.
       */
      inline ecs::Entity entity() {
        return _entity;
      }

      /**
       * Get the contact point at which the ray hit the entity..
       */
      inline ContactPoint point() {
        return _point;
      }

    private:
      ecs::Entity _entity;
      ContactPoint _point;
    };

  }
}