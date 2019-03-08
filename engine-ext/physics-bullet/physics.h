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

    void start() override;

    void fixedUpdate(const Frame& frame);

    void stop() override;

    void add(Entity entity) override;

    void remove(Entity entity) override;

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