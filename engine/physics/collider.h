#pragma once

// TODO: SCRUB THE SOURCE OF THESE
#define ColliderComponent 50

namespace wage {
  namespace physics {

    enum class ColliderType { box,
                              sphere,
                              mesh };

    class Collider {

    public:
      static Collider Box;

      static Collider Sphere;

      Collider() : Collider(ColliderType::box) {
      }

      Collider(ColliderType type) : _type(type) {
      }

      ~Collider() {}

      inline ColliderType type() const {
        return _type;
      }

    private:
      ColliderType _type;
    };

  }
}