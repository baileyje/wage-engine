#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>

#include "core/system.h"

class Physics : public System {

public:
  
  Physics();

  ~Physics();

  void init(Context* context);

  void fixedUpdate(Context* context);

  void deinit(Context* context);

private:
	btDefaultCollisionConfiguration* collisionConfiguration;
  
  btCollisionDispatcher* dispatcher;

  btBroadphaseInterface* overlappingPairCache;

  btSequentialImpulseConstraintSolver* solver;  

  btDiscreteDynamicsWorld* dynamicsWorld;  

  btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

#endif // PHYSICS_H