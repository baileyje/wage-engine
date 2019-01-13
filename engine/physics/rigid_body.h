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

    RigidBody(float mass = 0, RigidBodyType type = RigidBodyType::dynamic) : Component("RigidBody"), _mass(mass), _affectedByGravity(true), _type(type) {}

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

    inline RigidBodyType type() {
      return _type;
    }

    inline Vector force() {
      return _force;
    }

    inline Vector impulse() {
      return _impulse;
    }

    inline void clearImpulse() {
      _impulse = Vector();
    }

    inline void clearForce() {
      _force = Vector();
    }  

  private:

    float _mass;

    bool _affectedByGravity;

    Vector _impulse;

    Vector _force;
    
    // Vector newVelocity;

    // Vector acceleration;

    RigidBodyType _type;

    static ObjectPool<RigidBody> Pool;
    
  };

}

#endif //ENTITY_RIGID_BODY_H
