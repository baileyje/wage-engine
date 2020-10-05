#pragma once

#include "math/math.h"

// TODO: SCRUB THE SOURCE OF THESE
#define ColliderComponent 50

namespace wage {
  namespace physics {

    /**
     * Enumeration of collider types supported by the physics system.
     */
    enum class ColliderType {
      box,
      sphere,
      mesh
    };

    /**
     * Component class used to add a physics collider to an entity.
     * 
     * TODO: Give this its own transform.
     */
    class Collider {

    public:
      /**
       *  Create a default collider (hint, it's a box).
       */
      Collider() : Collider(ColliderType::box) {
      }

      /**
       * Create a collider with a specifc collider type.
       */
      Collider(ColliderType type) : type(type) {      
      }

      ~Collider() {}

      ColliderType type;

      math::Transform transform;

    };

  }
}