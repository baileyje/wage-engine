#ifndef ENTITY_RIGID_BODY_H
#define ENTITY_RIGID_BODY_H

#include <vector>

#include "memory/object_pool.h"
#include "memory/allocator.h"

#include "math/vector.h"

namespace wage {

  enum class RigidBodyType {
    dynamic, kinematic, immovable
  };

  class RigidBody {

  public:

    RigidBody(float mass = 0, RigidBodyType type = RigidBodyType::dynamic) : _mass(mass), _affectedByGravity(true), _type(type), _shouldStop(false) {}

    ~RigidBody() {}

    inline float mass() {
      return _mass;
    }

    inline void affectedByGravity(bool affected) {
      _affectedByGravity = affected;
    }

    inline bool isAffectedByGravity() {
      return _affectedByGravity;
    }

    inline void addForce(Vector force) {
      _force += force;
    }

    inline void addImpulse(Vector impulse) {
      _impulse += impulse;
    }

    inline RigidBodyType type() const {
      return _type;
    }

    inline Vector force() const {
      return _force;
    }

    inline Vector impulse() const {
      return _impulse;
    }

    inline Vector linearVelocity() const {
      return _linearVelocity;
    }

    inline void linearVelocity(Vector linearVelocity) {
      _linearVelocity = linearVelocity;
    }

    inline bool shouldStop() const {
      return _shouldStop;
    }

    inline void shouldStop(bool shouldStop) {
      _shouldStop = shouldStop;
    }

    inline void clearImpulse() {
      _impulse = Vector();
    }

    inline void clearForce() {
      _force = Vector();
    }

    inline void clearShouldStop() {
      _shouldStop = false;
    }

  private:

    float _mass;

    bool _affectedByGravity;

    Vector _impulse;

    Vector _force;

    Vector _linearVelocity;

    RigidBodyType _type;

    bool _shouldStop;
  };

}

#endif //ENTITY_RIGID_BODY_H
