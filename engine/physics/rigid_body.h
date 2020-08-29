#pragma once

#include <vector>

#include "memory/object_pool.h"
#include "memory/allocator.h"

#include "math/vector.h"
#include "ecs/common.h"

// TODO: SCRUB THE SOURCE OF THESE
#define RigidBodyComponent 51

namespace wage {
  namespace physics {

    enum class RigidBodyType {
      dynamic,
      kinematic,
      immovable
    };

    class RigidBody {

    public:
      RigidBody(float mass = 0, RigidBodyType type = RigidBodyType::dynamic) : _mass(mass), _affectedByGravity(true), _type(type), _shouldStop(false) {}

      ~RigidBody() {}

      inline float mass() const {
        return _mass;
      }

      inline void affectedByGravity(bool affected) {
        _affectedByGravity = affected;
      }

      inline bool isAffectedByGravity() const {
        return _affectedByGravity;
      }

      inline void addForce(math::Vector force) {
        _force += force;
        _shouldClearLinearVelocity = false;
      }

      inline void addImpulse(math::Vector impulse) {
        _impulse += impulse;
        _shouldClearLinearVelocity = false;
      }

      inline void addTorque(math::Vector torque) {
        _torque += torque;
        _shouldClearAngularVelocity = false;
      }

      inline void addTorqueImpulse(math::Vector impulse) {
        _torqueImpulse += impulse;
        _shouldClearAngularVelocity = false;
      }

      inline RigidBodyType type() const {
        return _type;
      }

      inline math::Vector force() const {
        return _force;
      }

      inline math::Vector impulse() const {
        return _impulse;
      }

      inline math::Vector torque() const {
        return _torque;
      }

      inline math::Vector torqueImpulse() const {
        return _torqueImpulse;
      }

      inline math::Vector linearVelocity() const {
        return _linearVelocity;
      }

      inline void linearVelocity(math::Vector linearVelocity) {
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
        _force = math::Vector3::Zero;
        _impulse = math::Vector3::Zero;
        _torque = math::Vector3::Zero;
        _torqueImpulse = math::Vector3::Zero;
      }

    private:
      float _mass;

      bool _affectedByGravity;

      math::Vector _impulse;

      math::Vector _force;

      math::Vector _torque;

      math::Vector _torqueImpulse;

      math::Vector _linearVelocity;

      RigidBodyType _type;

      bool _shouldStop;

      bool _shouldClearLinearVelocity;

      bool _shouldClearAngularVelocity;
    };

  }
}