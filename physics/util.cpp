#include "physics/util.h"


btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape) {
	// btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(cInfo);
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	body->setUserIndex(-1);
	return body;
}

btTransform fromTransform(Transform* transform) {
  btTransform btTransform;
  btTransform.setIdentity();
  btTransform.setOrigin(fromVector(transform->getPosition()));
  btQuaternion rotation(transform->rotation.y, transform->rotation.x, transform->rotation.z);
  btTransform.setRotation(rotation);
  return btTransform;
}