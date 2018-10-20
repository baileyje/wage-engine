#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>

#include <vector>

#include "core/system.h"

#include "physics/entity.h"

class Physics : public System {

public:
  
  Physics();

  ~Physics();

  void init(Context* context);

  void start(Context* context);

  void fixedUpdate(Context* context);

  void deinit(Context* context);

  void add(Entity* entity);

private:

  btCollisionShape* shapeFor(Entity* entity);

	btDefaultCollisionConfiguration collisionConfiguration;
  
  btCollisionDispatcher dispatcher;

  btDbvtBroadphase overlappingPairCache;

  btSequentialImpulseConstraintSolver solver;  

  btDiscreteDynamicsWorld dynamicsWorld;  

  std::vector<PhysicsEntity*> entities;
};

#endif // PHYSICS_H