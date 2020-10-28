#pragma once

#include "math/math.h"

// TODO: SCRUB THE SOURCE OF THESE
#define ColliderComponent 50

namespace wage {
  namespace physics {

    /**
     * Component class used to add a physics collider to an entity.
     */
    class Collider {

    public:
      /**
       * Enumeration of collider types supported by the physics system.
       */
      enum class Type {
        box,
        sphere,
        mesh
      };

      /**
       *  Create a default collider (hint, it's a box).
       */
      Collider() : Collider(Type::box) {
      }

      /**
       * Create a collider with a specifc collider type.
       */
      Collider(Type type) : type(type) {
      }

      ~Collider() {}

      Type type;

      math::Transform transform;
    };

  }
}