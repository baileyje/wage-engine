#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

#include <btBulletDynamicsCommon.h>

#include "entity/component/rigid_body.h"
#include "entity/component/collider.h"
#include "entity/store.h" // EntityReference :(
#include "physics/util.h"

class PhysicsEntity {

public:

  PhysicsEntity(EntityReference entity, btCollisionShape* shape, btRigidBody* rigidBody) 
    : PhysicsEntity(entity, shape, rigidBody, rigidBody) {
  }

  ~PhysicsEntity() {    
    if (rigidBody && rigidBody->getMotionState()) {
      delete rigidBody->getMotionState();
    }
    delete object; // NOT rigidBody
    delete shape;
  }  

  static PhysicsEntity* from(EntityReference entity, btDiscreteDynamicsWorld* dynamicsWorld);

  void applyForces();

  void updateEntityTransform();

  void updateShapeTransform();

  inline btRigidBody* getRigidBody() {
    return rigidBody;
  }

  inline btCollisionObject* getObject() {
    return object;
  }

private: 

  PhysicsEntity(EntityReference entity, btCollisionShape* shape, btRigidBody* rigidBody, btCollisionObject* object)
    : entity(entity), shape(shape), rigidBody(rigidBody), object(object) {    
  }

  static btCollisionShape* shapeFor(EntityReference entity);

  static btRigidBody* rigidBodyFor(RigidBody* rigidBody, const btTransform& startTransform, btCollisionShape* shape);
  
  EntityReference entity;

  btTransform getTransform(); 

  btCollisionShape* shape;

  btRigidBody* rigidBody;

  btCollisionObject* object;

};


#endif //PHYSICS_ENTITY_H
