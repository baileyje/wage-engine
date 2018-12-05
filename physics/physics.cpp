#include "physics/physics.h"

#include "entity/component/rigid_body.h"
#include "entity/component/collider.h"
#include "core/logger.h"
#include "core/system/context.h"

Physics::Physics() : System("Bullet Physics"), 
	dispatcher(&collisionConfiguration), 
	dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration)
{}

Physics::~Physics() {}

void Physics::init(SystemContext* context) {
	dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
	context->get<Messaging>()->listen<AddEntityMessage>(this);
	context->get<Messaging>()->listen<DestroyEntityMessage>(this);
}

void Physics::start(SystemContext* context) {
}

void Physics::on(AddEntityMessage& message) {
	add(message.getEntity());
}

void Physics::on(DestroyEntityMessage& message) {
	remove(message.getEntity());
}

void Physics::fixedUpdate(SystemContext* context) {
	// Get Physics up to speed
  for (auto physicsEntity : entities) {
		if (!physicsEntity->getEntity().isValid()) {
			continue;
		}
		physicsEntity->updateShapeTransform();
		physicsEntity->applyForces();	
	}

	// Run the Simulation
  dynamicsWorld.stepSimulation(context->getTimeStep(), 3);	

	// Get entities up to speed
	for (auto physicsEntity : entities) {
		if (!physicsEntity->getEntity().isValid()) {
			continue;
		}
		physicsEntity->updateEntityTransform();
	}
}

void Physics::deinit(SystemContext* context) {
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

void Physics::remove(EntityReference entity) {
	for (auto ent = entities.begin(); ent != entities.end(); ++ent) {
		if ((*ent)->getEntity() == entity) {
			if ((*ent)->getRigidBody()) {
				dynamicsWorld.removeRigidBody((*ent)->getRigidBody());
			} else {
				dynamicsWorld.removeCollisionObject((*ent)->getObject());
			}		
			entities.erase(ent);
			return;
		}
	}	
}

