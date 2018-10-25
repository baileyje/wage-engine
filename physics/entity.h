#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

#include <btBulletDynamicsCommon.h>

#include "entity/component/rigid_body.h"
#include "entity/component/collider.h"
#include "entity/entity.h"
#include "physics/util.h"

class PhysicsEntity {

public:

  PhysicsEntity(Entity* entity, btCollisionShape* shape, btRigidBody* rigidBody) 
    : entity(entity), shape(shape), rigidBody(rigidBody) {
  }

  ~PhysicsEntity() {    
    if (rigidBody && rigidBody->getMotionState()) {
      delete rigidBody->getMotionState();
    }
    delete rigidBody;
    delete shape;
  }

  static btCollisionShape* shapeFor(Entity* entity);

  static PhysicsEntity* from(Entity* entity, btDiscreteDynamicsWorld* dynamicsWorld);

  btTransform getTransform();

  void applyForces();

  void updateTransform();

  void updateShapeTransform();

  inline btRigidBody* getRigidBody() {
    return rigidBody;
  }

private: 

  Entity* entity;

  btCollisionShape* shape;

  btRigidBody* rigidBody;
  
};


#endif //PHYSICS_ENTITY_H
