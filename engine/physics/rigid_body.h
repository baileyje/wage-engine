#ifndef ENTITY_RIGID_BODY_H
#define ENTITY_RIGID_BODY_H

#include <vector>

#include "memory/object_pool.h"
#include "memory/allocator.h"

#include "entity/component.h"
#include "math/vector.h"

namespace wage {

  enum class RigidBodyType {
    dynamic, kinematic, immovable
  };

  class RigidBody : public Component {

  public:

    template <typename... Args>
    static RigidBody* create(Args... args) {
      auto ref = Pool.create(args...);
      auto ptr = ref.get();
      return ptr;
    }

    RigidBody(float mass = 0, RigidBodyType type = RigidBodyType::dynamic) : Component("RigidBody"), mass_(mass), affectedByGravity_(true), type_(type) {}

    ~RigidBody() {}

    inline float mass() {
      return mass_;
    }

    inline void setAffectedByGravity(bool affected) {
      affectedByGravity_ = affected;
    }

    inline bool isAffectedByGravity() {
      return affectedByGravity_;
    }

    inline void addForce(Vector force) {
      force_ += force;
    }

    inline void addImpulse(Vector impulse) {
      impulse_ += impulse;
    }

    inline RigidBodyType type() {
      return type_;
    }

    inline Vector force() {
      return force_;
    }

    inline Vector impulse() {
      return impulse_;
    }

    inline void clearImpulse() {
      impulse_ = Vector();
    }

    inline void clearForce() {
      force_ = Vector();
    }  

  private:

    float mass_;

    bool affectedByGravity_;

    Vector impulse_;

    Vector force_;
    
    // Vector newVelocity;

    // Vector acceleration;

    RigidBodyType type_;

    static ObjectPool<RigidBody> Pool;
    
  };

}

#endif //ENTITY_RIGID_BODY_H
