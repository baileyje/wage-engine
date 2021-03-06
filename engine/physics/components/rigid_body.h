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

  
    /**
     * Component class used to add a physics rigid body to an entity.
     */
    class RigidBody {

    public:
      /**
       * Enumeration of rigid body types.
       */
      enum class Type {
        dynamic,
        kinematic,
        immovable
      };

      /**
       * The mass of the rigid body.
       */
      float mass;

      /**
       * Whether this rigid body is affected by gravity.
       */
      bool affectedByGravity;

      /**
       * Get the rigid body type.
       */
      Type type;

      /**
       * Create a rigid body with a specified mass and type.
       */
      RigidBody(float mass = 0, Type type = Type::dynamic) : mass(mass), affectedByGravity(true), type(type), _shouldStop(false) {}

      ~RigidBody() {}

      /**
       * Add a force to this rigid body.
       */
      inline void addForce(math::Vector force) {
        _force += force;
        _shouldClearLinearVelocity = false;
      }

      /**
       * Add an impulse to this rigid body.
       */
      inline void addImpulse(math::Vector impulse) {
        _impulse += impulse;
        _shouldClearLinearVelocity = false;
      }

      /**
       * Add a torque to this rigid body.
       */
      inline void addTorque(math::Vector torque) {
        _torque += torque;
        _shouldClearAngularVelocity = false;
      }

      /**
       * Add a torque impulse to this rigid body.
       */
      inline void addTorqueImpulse(math::Vector impulse) {
        _torqueImpulse += impulse;
        _shouldClearAngularVelocity = false;
      }

      /**
       * Get the current force applied to this rigid body.
       */
      inline math::Vector force() const {
        return _force;
      }

      /**
       * Get the impulse applied to this rigid body.
       */
      inline math::Vector impulse() const {
        return _impulse;
      }

      /**
       * Get the torque applied to this rigid body.
       */
      inline math::Vector torque() const {
        return _torque;
      }

      /**
       * Get the torque impulse applied to this rigid body.
       */
      inline math::Vector torqueImpulse() const {
        return _torqueImpulse;
      }

      /**
       * Get the linear velocity of this rigid body.
       */
      inline math::Vector linearVelocity() const {
        return _linearVelocity;
      }

      /**
       * Set the linear velocity for this rigid body.
       */
      inline void linearVelocity(math::Vector linearVelocity) {
        _linearVelocity = linearVelocity;
      }

      /**
       * Indicator of whther this rigid body should stop moving.
       */
      inline bool shouldStop() const {
        return _shouldStop;
      }

      /**
       * Remove the should stop indicator from this rigid body.
       */
      inline void clearShouldStop() {
        _shouldStop = false;
      }

      /**
       * Remove the linear velocity from this rigid body.
       */
      inline void clearLinearVelocity() {
        _shouldClearLinearVelocity = true;
      }

      /**
       * Determine whether the linear velocity should be removed from this rigid body.
       */
      inline bool shouldClearLinearVelocity() const {
        return _shouldClearLinearVelocity;
      }

      /**
       * Remove the angular velocity from this rigid body.
       */
      inline void clearAngularVelocity() {
        _shouldClearAngularVelocity = true;
      }

      /**
       * Determine whether the angular velocity should be removed from this rigid body.
       */
      inline bool shouldClearAngularVelocity() const {
        return _shouldClearAngularVelocity;
      }

      /**
       * Set whther this rigid body should stop.
       */
      inline void shouldStop(bool shouldStop) {
        _shouldStop = shouldStop;
      }

      /**
       * Clear all forces applied to this rigid body. This is called at the end of the frame to start the next
       *  frame without any force being applied.
       */
      inline void clearForces() {
        _force = math::Vector3::Zero;
        _impulse = math::Vector3::Zero;
        _torque = math::Vector3::Zero;
        _torqueImpulse = math::Vector3::Zero;
      }

    private:
  
      math::Vector _impulse;

      math::Vector _force;

      math::Vector _torque;

      math::Vector _torqueImpulse;

      math::Vector _linearVelocity;

      bool _shouldStop;

      bool _shouldClearLinearVelocity;

      bool _shouldClearAngularVelocity;
    };

  }
}