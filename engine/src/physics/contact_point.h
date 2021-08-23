#pragma once

#include "math/vector.h"

namespace wage {

  namespace physics {

    /**
     * A contact point between two colliding bodies in the world, or the contact point for ray cast hit.
     */
    class ContactPoint {
    public:
      ContactPoint() {}
      ContactPoint(math::Vector3 point, math::Vector3 normal) : _point(point), _normal(normal) {}
      ContactPoint(const ContactPoint& cp) : ContactPoint(cp._point, cp._normal) {}

      /**
       * Get the contact point in world space.
       */
      inline math::Vector3 point() {
        return _point;
      }
      /**
       * Get the normal at the point of contact.
       */
      inline math::Vector3 normal() {
        return _normal;
      }

    private:
      math::Vector3 _point;
      math::Vector3 _normal;
    };

  }
}
