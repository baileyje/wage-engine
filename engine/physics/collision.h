#pragma once

#include <vector>

#include "ecs/entity.h"
#include "math/vector.h"

namespace wage {

  namespace physics {

    class ContactPoint {
    public:
      ContactPoint() {}
      ContactPoint(math::Vector3 point, math::Vector3 normal) : _point(point), _normal(normal) {}
      ContactPoint(const ContactPoint& cp) : ContactPoint(cp._point, cp._normal) {}

      inline math::Vector3
      point() {
        return _point;
      }

      inline math::Vector3 normal() {
        return _normal;
      }

    private:
      math::Vector3 _point;
      math::Vector3 _normal;
    };

    class Collision {

    public:
      Collision(ecs::Entity entity, ecs::Entity otherEntity, std::vector<ContactPoint> contacts) : _entity(entity), _otherEntity(otherEntity), _contacts(contacts) {}

      Collision(const Collision& cp) : Collision(cp._entity, cp._otherEntity, cp._contacts) {}

      inline ecs::Entity entity() {
        return _entity;
      }

      inline ecs::Entity otherEntity() {
        return _otherEntity;
      }

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
