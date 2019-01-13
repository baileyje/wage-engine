#include "physics-bullet/physics.h"

#include "physics/rigid_body.h"
#include "physics/collider.h"
#include "core/logger.h"
#include "core/system/context.h"

namespace wage {

	BulletPhysics::BulletPhysics() : Physics(), 
		dispatcher(&collisionConfiguration), 
		dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration)
	{}

	BulletPhysics::~BulletPhysics() {}

	void BulletPhysics::init(SystemContext* context) {
    Physics::init(context);
		dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
	}

	void BulletPhysics::fixedUpdate(SystemContext* context) {
		// Get Physics up to speed
		for (auto physicsEntity : entities) {
			if (!physicsEntity->entity().isValid()) {
				continue;
			}
			physicsEntity->updateShapeTransform();
			physicsEntity->applyForces();	
		}

		// Run the Simulation
		dynamicsWorld.stepSimulation(context->timeStep(), 3);	

		// Get entities up to speed
		for (auto physicsEntity : entities) {
			if (!physicsEntity->entity().isValid()) {
				continue;
			}
			physicsEntity->updateEntityTransform();
		}
	}

	void BulletPhysics::deinit(SystemContext* context) {
		// TODO: Evaluate memory free, etc.
		// for (auto entity : entities) {
		// 	if (entity->getObject()) {
		// 		dynamicsWorld.removeCollisionObject(entity->getObject());
		// 	}
		// 	delete entity;
		// }
	}

	void BulletPhysics::add(EntityReference entity) {	
		entities.push_back(PhysicsEntity::from(entity, &dynamicsWorld));
	}

	void BulletPhysics::remove(EntityReference entity) {
		for (auto ent = entities.begin(); ent != entities.end(); ++ent) {
			if ((*ent)->entity() == entity) {
				if ((*ent)->rigidBody()) {
					dynamicsWorld.removeRigidBody((*ent)->rigidBody());
				} else {
					dynamicsWorld.removeCollisionObject((*ent)->object());
				}		
				entities.erase(ent);
				return;
			}
		}	
	}

}
