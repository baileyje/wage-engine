#include "physics/entity.h"

btCollisionShape* PhysicsEntity::shapeFor(Entity* entity) {
  Collider* collider = entity->get<Collider>();
  if (!collider) {
    return new btEmptyShape();
  }
  switch(collider->getType()) {
    // TODO: Support more shapes
    case box: {
      btVector3 halfExtents = btVector3(entity->getTransform()->getScale()->x/2.0, entity->getTransform()->getScale()->y/2.0, entity->getTransform()->getScale()->z/2.0);			
      return new btBoxShape(halfExtents);
    }
    default: return nullptr;
  }	
}

btRigidBody* PhysicsEntity::rigidBodyFor(RigidBody* rigidBody, const btTransform& startTransform, btCollisionShape* shape) {
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
	btVector3 localInertia(0, 0, 0);
	if (rigidBody->getMass() != 0.f) {
		shape->calculateLocalInertia(rigidBody->getMass(), localInertia);
  }    
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(rigidBody->getMass(), myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(cInfo);  
  if (rigidBody->getType() == kinematic) {
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  } else if (rigidBody->getType() == immovable) {
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
  }
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	body->setUserIndex(-1);
	return body;
}

PhysicsEntity* PhysicsEntity::from(Entity* entity, btDiscreteDynamicsWorld* dynamicsWorld) {
  btCollisionShape* shape = shapeFor(entity);
  btRigidBody* body = nullptr;
  btCollisionObject* object = nullptr;
  RigidBody* entityBody = entity->get<RigidBody>();
  if (entityBody) {
    body = rigidBodyFor(entityBody, fromTransform(entity->getTransform()), shape);
    dynamicsWorld->addRigidBody(body);
    // Must come after added to world.    
    if (!entityBody->isAffectedByGravity()) {
      body->setGravity(btVector3(0, 0, 0));
    }
    object = body;
  }	else {
    object = new btCollisionObject();
    object->setCollisionShape(shape);
    dynamicsWorld->addCollisionObject(object);
  }
  return new PhysicsEntity(entity, shape, body, object);
}

 btTransform PhysicsEntity::getTransform() {
  btTransform transform;
  if (rigidBody && rigidBody->getMotionState()) {
    rigidBody->getMotionState()->getWorldTransform(transform);
  } else {
    transform = rigidBody->getWorldTransform();
  }
  return transform;
}

void PhysicsEntity::applyForces() {
  if (!rigidBody) {
    return;
  }
  RigidBody* entityBody = entity->get<RigidBody>();
  if (!entityBody) {
    return;
  }
  btVector3 impulse = fromVector(entityBody->getImpulse());
  if (impulse.length() > 0) {
    rigidBody->applyCentralImpulse(impulse);	
    entityBody->clearImpulse();
  }
  btVector3 force = fromVector(entityBody->getForce());
  if (force.length() > 0) {
    rigidBody->applyCentralForce(force);	
  }			
  rigidBody->activate(true);
}

void PhysicsEntity::updateEntityTransform() {
  if (!rigidBody) {
    return;
  }
  btTransform transform = getTransform();
  entity->getTransform()->setPosition(fromBTVector(transform.getOrigin()));
  btQuaternion rotation = transform.getRotation();
  btScalar yawZ, pitchY, rollX;
  rotation.getEulerZYX(yawZ, pitchY, rollX);
  entity->getTransform()->setRotation(Vector(
    btDegrees(rollX), 
    btDegrees(pitchY), 
    btDegrees(yawZ)
  ));
}

void PhysicsEntity::updateShapeTransform() {
  if (!rigidBody) {
    return;
  }
  RigidBody* entityBody = entity->get<RigidBody>();
  if (entityBody->getType() == dynamic) {
    return;
  }  
  btTransform transform = fromTransform(entity->getTransform());
  rigidBody->getMotionState()->setWorldTransform(transform);
}