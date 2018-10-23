#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

#include <btBulletDynamicsCommon.h>

#include "entity/component/rigid_body.h"
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

  btTransform getTransform() {
		btTransform transform;
		if (rigidBody && rigidBody->getMotionState()) {
			rigidBody->getMotionState()->getWorldTransform(transform);
		} else {
			transform = rigidBody->getWorldTransform();
		}
    return transform;
  }

  void applyForces() {
    if (!rigidBody) {
      return;
    }
    RigidBody* entityBody = (RigidBody*)entity->getComponent("RigidBody");
		if (!entityBody) {
			return;
		}
		btVector3 impulse = fromVector(&entityBody->currentForce);
		if (impulse.length() > 0) {
			rigidBody->applyCentralImpulse(impulse);	
			entityBody->currentForce = Vector();
		}		
		rigidBody->activate(true);
  }

  void updateTransform() {
    if (!rigidBody) {
      return;
    }
    btTransform transform = getTransform();		
		entity->getTransform()->setPosition(fromBTVector(&transform.getOrigin()));
		btQuaternion rotation = transform.getRotation();
		btScalar yawZ, pitchY, rollX;
		rotation.getEulerZYX(yawZ, pitchY, rollX);
		entity->getTransform()->setRotation(Vector(rollX, pitchY, yawZ));
  }

  btRigidBody* getRigidBody() {
    return rigidBody;
  }

private: 

  Entity* entity;

  btCollisionShape* shape;

  btRigidBody* rigidBody;

};


#endif //PHYSICS_ENTITY_H
