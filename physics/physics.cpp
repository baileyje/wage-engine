#include "physics/physics.h"

btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape) {
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	printf("Dyn: %d\n", isDynamic);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
	btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
	body->setWorldTransform(startTransform);
#endif  //

	body->setUserIndex(-1);
	return body;
}

btBoxShape* createBoxShape(const btVector3& halfExtents) {
	btBoxShape* box = new btBoxShape(halfExtents);
	return box;
}




Physics::Physics() : 
	System("bullet_physics"), 
	dispatcher(&collisionConfiguration), 
	dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration)
{}

Physics::~Physics() {}


void Physics::init(Context* context) {
  dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
}

void Physics::start(Context* context) {
	int usrIdx = 0; // Super jank
  for (auto entity : *context->getEntities()) {
		btBoxShape* shape = createBoxShape(btVector3(entity->getTransform()->getScale()->x/2.0, entity->getTransform()->getScale()->y/2.0, entity->getTransform()->getScale()->z/2.0));
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(
			btVector3(
				(btScalar)entity->getTransform()->getPosition()->x, 
				(btScalar)entity->getTransform()->getPosition()->y, 
				(btScalar)entity->getTransform()->getPosition()->z
			)
		);		
		btRigidBody* body = createRigidBody(1.0, transform, shape);
		body->setUserIndex(usrIdx++);
		bodies.push_back(body);
		dynamicsWorld.addRigidBody(body);
	}
	btBoxShape* grndShape = createBoxShape(btVector3(100, 0.1, 100));
	btTransform grndTransform;
	grndTransform.setIdentity();
	grndTransform.setOrigin(btVector3(0, 0.0, 0));
	btRigidBody* ground = createRigidBody(0.0, grndTransform, grndShape);
	ground->setUserIndex(usrIdx++);
	// bodies.push_back(ground);
	dynamicsWorld.addRigidBody(ground);
}

void Physics::fixedUpdate(Context* context) {
  // printf("Bullet Phys!\n");
  dynamicsWorld.stepSimulation(context->timeStep, 10);	
	for (int j = dynamicsWorld.getNumCollisionObjects() - 1; j >= 0; j--) {
			btCollisionObject* obj = dynamicsWorld.getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btMotionState* motionState = body->getMotionState();
			btTransform transform;
			if (body && body->getMotionState()) {
				body->getMotionState()->getWorldTransform(transform);
			}
			else {
				transform = obj->getWorldTransform();
			}
			int userIdx = body->getUserIndex();
			std::vector<Entity*> entities = *context->getEntities();
			if (userIdx >= entities.size()) {
				continue;
			}
			Entity* entity = (*context->getEntities())[userIdx];
			// printf("X: %f, Y: %f, Z: %f \n", transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());
			entity->getTransform()->setPosition(Vector(transform.getOrigin().getX(), transform.getOrigin().getY(), 0.0 /*transform.getOrigin().getZ()*/ ));
			btQuaternion rotation = transform.getRotation();
			Vector* entityRotation = entity->getTransform()->getRotation();
			btScalar yawZ, pitchY, rollX;
			rotation.getEulerZYX(yawZ, pitchY, rollX);
			entityRotation->x = rollX;
			entityRotation->y = pitchY;
			entityRotation->z = yawZ;
	}
}

void Physics::deinit(Context* context) {
  for (int i = dynamicsWorld.getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = dynamicsWorld.getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		dynamicsWorld.removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++) {
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}
	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}