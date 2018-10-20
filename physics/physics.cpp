#include "physics/physics.h"

#include "entity/component/rigid_body.h"

btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape) {
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	bool isDynamic = (mass != 0.f);
	
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

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

Physics::Physics() : System("bullet_physics"), 
	dispatcher(&collisionConfiguration), 
	dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration)
{}

Physics::~Physics() {}


void Physics::init(Context* context) {
  dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
	// dynamicsWorld.setGravity(btVector3(0, 0, 0));
}

void Physics::start(Context* context) {
	int usrIdx = 0; // Super jank
  for (auto entity : *context->getEntities()) {
		RigidBody* entityBody = (RigidBody*)entity->getComponent("RigidBody");
		if (!entityBody) {
			continue;
		}
		btBoxShape* shape = createBoxShape(btVector3(entity->getTransform()->getScale()->x/2.0, entity->getTransform()->getScale()->y/2.0, entity->getTransform()->getScale()->z/2.0));
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(
			btVector3(
				(btScalar)entity->getTransform()->position.x, 
				(btScalar)entity->getTransform()->position.y, 
				(btScalar)entity->getTransform()->position.z
			)
		);		
		btRigidBody* body = createRigidBody(entityBody->mass, transform, shape);
		body->setUserIndex(usrIdx++);
		bodies.push_back(body);
		dynamicsWorld.addRigidBody(body);
	}	
}

void Physics::fixedUpdate(Context* context) {
  // printf("Bullet Phys!\n");
	for (int j = dynamicsWorld.getNumCollisionObjects() - 1; j >= 0; j--) {
		btCollisionObject* obj = dynamicsWorld.getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		body->activate(true);
		int userIdx = body->getUserIndex();
		std::vector<Entity*> entities = *context->getEntities();
		if (userIdx >= entities.size()) {
			printf("WTF!\n");
			continue;
		}
		Entity* entity = (*context->getEntities())[userIdx];
		RigidBody* entityBody = (RigidBody*)entity->getComponent("RigidBody");
		if (!entityBody) {
			printf("WTF2!\n");
			continue;
		}
		body->activate(true);
		// printf("x: %f y: %f, z: %f\n", entityBody->currentForce.x, entityBody->currentForce.y, entityBody->currentForce.z);
		btVector3 impulse(
				(btScalar)entityBody->currentForce.x, 
				(btScalar)entityBody->currentForce.y, 
				(btScalar)entityBody->currentForce.z
			);
		if (impulse.length() > 0) {
			printf("x: %;f y: %lf, z: %lf\n", impulse.x(), impulse.y(), impulse.z());
			body->applyCentralImpulse(impulse);	
			entityBody->currentForce = Vector();
		}		
		body->activate(true);
	}	
  dynamicsWorld.stepSimulation(context->timeStep, 3);	
	// printf("CNT: %d\n", dynamicsWorld.getNumCollisionObjects());
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
		entity->getTransform()->setPosition(Vector(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ() ));
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