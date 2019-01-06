#ifndef ENTITY_RIGID_BODY_H
#define ENTITY_RIGID_BODY_H

#include <vector>

#include "engine/memory/object_pool.h"
#include "engine/memory/allocator.h"

#include "engine/entity/component.h"
#include "engine/math/vector.h"

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

    RigidBody(float mass = 0, RigidBodyType type = RigidBodyType::dynamic) : Component("RigidBody"), mass(mass), affectedByGravity(true), type(type) {}

    ~RigidBody() {}

    inline float getMass() {
      return mass;
    }

    inline void setAffectedByGravity(bool affected) {
      affectedByGravity = affected;
    }

    inline bool isAffectedByGravity() {
      return affectedByGravity;
    }

    inline void addForce(Vector force) {
      this->force += force;
    }

    inline void addImpulse(Vector impulse) {
      this->impulse += impulse;
    }

    inline RigidBodyType getType() {
      return type;
    }

    inline Vector getForce() {
      return force;
    }

    inline Vector getImpulse() {
      return impulse;
    }

    inline void clearImpulse() {
      impulse = Vector();
    }

    inline void clearForce() {
      force = Vector();
    }  

  private:

    float mass;

    bool affectedByGravity;

    Vector impulse;

    Vector force;
    
    // Vector newVelocity;

    // Vector acceleration;

    RigidBodyType type;

    static ObjectPool<RigidBody> Pool;
    
  };

}

#endif //ENTITY_RIGID_BODY_H
