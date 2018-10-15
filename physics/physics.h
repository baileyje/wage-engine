#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>

#include "core/system.h"

class Physics : public System {

public:
  
  Physics();

  ~Physics();

  void init(Context* context);

  void start(Context* context);

  void fixedUpdate(Context* context);

  void deinit(Context* context);

private:

	btDefaultCollisionConfiguration collisionConfiguration;
  
  btCollisionDispatcher dispatcher;

  btDbvtBroadphase overlappingPairCache;

  btSequentialImpulseConstraintSolver solver;  

  btDiscreteDynamicsWorld dynamicsWorld;  

  btAlignedObjectArray<btCollisionShape*> collisionShapes;

  btAlignedObjectArray<btRigidBody*> bodies;
};

#endif // PHYSICS_H