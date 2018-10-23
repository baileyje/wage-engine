#include "physics/physics.h"

#include "entity/component/rigid_body.h"
#include "entity/component/collider.h"


Physics::Physics() : System("bullet_physics"), 
	dispatcher(&collisionConfiguration), 
	dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration)
{}

Physics::~Physics() {}

void Physics::init(Context* context) {
  dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
}

void Physics::start(Context* context) {
	for (auto entity : *context->getEntities()) {
		add(entity);
	}	
}

void Physics::fixedUpdate(Context* context) {
  // printf("Bullet Phys!\n");
	for (auto physicsEntity : entities) {
		physicsEntity->applyForces();
	}

  dynamicsWorld.stepSimulation(context->timeStep, 3);	

	for (auto entity : entities) {
		entity->updateTransform();
	}
}

void Physics::deinit(Context* context) {
  for (auto entity : entities) {
		dynamicsWorld.removeCollisionObject(entity->getRigidBody());
		delete entity;
	}
}

void Physics::add(Entity* entity) {	
	btCollisionShape* shape = shapeFor(entity);
	btRigidBody* body = nullptr;
	RigidBody* entityBody = (RigidBody*)entity->getComponent("RigidBody");
	if (entityBody) {
		body = createRigidBody(entityBody->mass, fromTransform(entity->getTransform()), shape);
		dynamicsWorld.addRigidBody(body);
	}	else {
		btCollisionObject* object = new btCollisionObject();
		object->setCollisionShape(shape);
		dynamicsWorld.addCollisionObject(object);
	}
	entities.push_back(new PhysicsEntity(entity, shape, body));
	
}

btCollisionShape* Physics::shapeFor(Entity* entity) {
	Collider* collider = (Collider*)entity->getComponent("Collider");
	if (!collider) {
		printf("No Collider!!\n");
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