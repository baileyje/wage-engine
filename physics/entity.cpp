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

PhysicsEntity* PhysicsEntity::from(Entity* entity, btDiscreteDynamicsWorld* dynamicsWorld) {
  btCollisionShape* shape = shapeFor(entity);
  btRigidBody* body = nullptr;
  RigidBody* entityBody = entity->get<RigidBody>();
  if (entityBody) {
    body = createRigidBody(entityBody, fromTransform(entity->getTransform()), shape);
    dynamicsWorld->addRigidBody(body);
    // Must come after added to world.
    if (!entityBody->isAffectedByGravity()) {
      body->setGravity(btVector3(0, 0, 0));
    }
  }	else {
    btCollisionObject* object = new btCollisionObject();
    object->setCollisionShape(shape);
    dynamicsWorld->addCollisionObject(object);
  }
  return new PhysicsEntity(entity, shape, body);
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
    // entityBody->clearForce();
  }			
  rigidBody->activate(true);
}

void PhysicsEntity::updateTransform() {
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
  // rigidBody->setWorldTransform(transform);
  // transform.setOrigin(fromVector(entity->getTransform()->getPosition()));
  // btQuaternion rotation(
  //   btRadians(entity->getTransform()->getRotation()->y), 
  //   btRadians(entity->getTransform()->getRotation()->x), 
  //   btRadians(entity->getTransform()->getRotation()->z)
  // );
  // transform.setRotation(rotation);
}