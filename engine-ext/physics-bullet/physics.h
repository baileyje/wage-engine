#ifndef PHYSICS_BULLET_H
#define PHYSICS_BULLET_H

#include <btBulletDynamicsCommon.h>

#include <vector>

#include "engine/core/system.h"
#include "engine/messaging/messaging.h"
#include "engine/physics/physics.h"

#include "engine-ext/physics-bullet/entity.h"

namespace wage {

  class BulletPhysics : public Physics {

  public:
    
    BulletPhysics();

    virtual ~BulletPhysics();

    LIFECYCLE_FUNC(init)

    LIFECYCLE_FUNC(fixedUpdate)

    LIFECYCLE_FUNC(deinit)

    void add(EntityReference entity);

    void remove(EntityReference entity);

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