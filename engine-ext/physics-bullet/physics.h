#ifndef PHYSICS_BULLET_H
#define PHYSICS_BULLET_H

#include <btBulletDynamicsCommon.h>

#include <vector>

#include "ecs/system.h"
#include "messaging/messaging.h"
#include "physics/physics.h"

#include "physics-bullet/entity.h"

namespace wage {

  class BulletPhysics : public Physics {

  public:
    
    BulletPhysics();

    virtual ~BulletPhysics();

    LIFECYCLE_FUNC(init)

    LIFECYCLE_FUNC(fixedUpdate)

    LIFECYCLE_FUNC(deinit)

    void add(Entity entity);

    void remove(Entity entity);

  private:

    btDefaultCollisionConfiguration collisionConfiguration;
    
    btCollisionDispatcher dispatcher;

    btDbvtBroadphase overlappingPairCache;

    btSequentialImpulseConstraintSolver solver;  

    btDiscreteDynamicsWorld dynamicsWorld;  

    std::vector<PhysicsEntity*> entities;

  };

}

#endif // PHYSICS_BULLET_H