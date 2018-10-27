#include "physics/physics.h"

#include "entity/component/rigid_body.h"
#include "entity/component/collider.h"


Physics::Physics() : System("bullet_physics"), 
	dispatcher(&collisionConfiguration), 
	dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration)
{}

Physics::~Physics() {}

void Physics::init(Context* context) {
	printf("Initializing Physics.\n");
  dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
}

void Physics::start(Context* context) {
	printf("Starting Physics.\n");
	for (auto entity : *context->getEntities()) {
		add(entity);
	}	
}

void Physics::fixedUpdate(Context* context) {
	// Get Physics up to speed
  for (auto physicsEntity : entities) {
		physicsEntity->updateShapeTransform();
		physicsEntity->applyForces();	
	}

	// Run the Simulation
  dynamicsWorld.stepSimulation(context->timeStep, 3);	

	// Get entities up to speed
	for (auto entity : entities) {
		entity->updateEntityTransform();
	}
}

void Physics::deinit(Context* context) {
	printf("Deinitializing Physics.\n");
  for (auto entity : entities) {
		if (entity->getObject()) {
			dynamicsWorld.removeCollisionObject(entity->getObject());
		}
		delete entity;
	}
}

void Physics::add(Entity* entity) {	
	entities.push_back(PhysicsEntity::from(entity, &dynamicsWorld));
}

