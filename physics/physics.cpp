#include "physics/physics.h"

#include "entity/component/rigid_body.h"
#include "entity/component/collider.h"
#include "core/logger.h"
#include "core/context.h"

Physics::Physics() : System("bullet_physics"), 
	dispatcher(&collisionConfiguration), 
	dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration)
{}

Physics::~Physics() {}

void Physics::init(Context* context) {
	Logger::info("Initializing Physics.");
  dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
}

void Physics::start(Context* context) {
	Logger::info("Starting Physics.");
	int idx = 0;
	for (EntityReference  entity : *context->get<EntityManager>()) {		
		add(entity);
		// for (auto child : *entity->getChildren()) {
		// 	add(child);
		// }
	}	
}

void Physics::fixedUpdate(Context* context) {
	// Get Physics up to speed
  for (auto physicsEntity : entities) {
		physicsEntity->updateShapeTransform();
		physicsEntity->applyForces();	
	}

	// Run the Simulation
  dynamicsWorld.stepSimulation(context->getTimeStep(), 3);	

	// Get entities up to speed
	for (auto entity : entities) {
		entity->updateEntityTransform();
	}
}

void Physics::deinit(Context* context) {
	Logger::info("Deinitializing Physics.");
  for (auto entity : entities) {
		if (entity->getObject()) {
			dynamicsWorld.removeCollisionObject(entity->getObject());
		}
		delete entity;
	}
}

void Physics::add(EntityReference entity) {	
	entities.push_back(PhysicsEntity::from(entity, &dynamicsWorld));
}

