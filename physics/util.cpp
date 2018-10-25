#include "physics/util.h"



btRigidBody* createRigidBody(RigidBody* rigidBody, const btTransform& startTransform, btCollisionShape* shape) {
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

btTransform fromTransform(Transform* transform) {
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(fromVector(*transform->getPosition()));
  btQuaternion rotation(
    btRadians(transform->rotation.y), 
    btRadians(transform->rotation.x), 
    btRadians(transform->rotation.z)
  );
  btTransform.setRotation(rotation);
  return btTransform;
}