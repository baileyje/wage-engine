#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>

#include <vector>

#include "core/system.h"

#include "physics/entity.h"

#include "messaging/messaging.h"

class Physics : public System, MessageListener<AddEntityMessage>, MessageListener<DestroyEntityMessage> {

public:
  
  Physics();

  ~Physics();

  LIFECYCLE_FUNC(init)

  LIFECYCLE_FUNC(start)

  LIFECYCLE_FUNC(fixedUpdate)

  LIFECYCLE_FUNC(deinit)

  void add(EntityReference entity);

  void remove(EntityReference entity);

  void on(AddEntityMessage& message);
  
  void on(DestroyEntityMessage& message);

private:

  // btCollisionShape* shapeFor(Entity* entity);

	btDefaultCollisionConfiguration collisionConfiguration;
  
  btCollisionDispatcher dispatcher;

  btDbvtBroadphase overlappingPairCache;

  btSequentialImpulseConstraintSolver solver;  

  btDiscreteDynamicsWorld dynamicsWorld;  

  std::vector<PhysicsEntity*> entities;

};

#endif // PHYSICS_H