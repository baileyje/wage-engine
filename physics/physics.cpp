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
  for (auto physicsEntity : entities) {
		physicsEntity->updateShapeTransform();
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
	entities.push_back(PhysicsEntity::from(entity, &dynamicsWorld));
}

