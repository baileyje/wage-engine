#ifndef ENTITY_RIGID_BODY_H
#define ENTITY_RIGID_BODY_H

#include <vector>

#include "memory/object_pool.h"
#include "memory/allocator.h"

#include "math/vector.h"

namespace wage {

  enum class RigidBodyType {
    dynamic,
    kinematic,
    immovable
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
      _shouldClearLinearVelocity = false;
    }

    inline void addImpulse(Vector impulse) {
      _impulse += impulse;
      _shouldClearLinearVelocity = false;
    }

    inline void addTorque(Vector torque) {
      _torque += torque;
      _shouldClearAngularVelocity = false;
    }

    inline void addTorqueImpulse(Vector impulse) {
      _torqueImpulse += impulse;
      _shouldClearAngularVelocity = false;
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

    inline Vector torque() const {
      return _torque;
    }

    inline Vector torqueImpulse() const {
      return _torqueImpulse;
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

    inline void clearShouldStop() {
      _shouldStop = false;
    }

    inline void clearLinearVelocity() {
      _shouldClearLinearVelocity = true;
    }

    inline bool shouldClearLinearVelocity() const {
      return _shouldClearLinearVelocity;
    }

    inline void clearAngularVelocity() {
      _shouldClearAngularVelocity = true;
    }

    inline bool shouldClearAngularVelocity() const {
      return _shouldClearAngularVelocity;
    }

    inline void shouldStop(bool shouldStop) {
      _shouldStop = shouldStop;
    }

    inline void clearForces() {
      _force = Vector3::Zero;
      _impulse = Vector3::Zero;
      _torque = Vector3::Zero;
      _torqueImpulse = Vector3::Zero;
    }

  private:
    float _mass;

    bool _affectedByGravity;

    Vector _impulse;

    Vector _force;

    Vector _torque;

    Vector _torqueImpulse;

    Vector _linearVelocity;

    RigidBodyType _type;

    bool _shouldStop;

    bool _shouldClearLinearVelocity;

    bool _shouldClearAngularVelocity;
  };
}

#endif //ENTITY_RIGID_BODY_H
