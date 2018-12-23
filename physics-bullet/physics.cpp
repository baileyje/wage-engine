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

	void BulletPhysics::deinit(SystemContext* context) {
		for (auto entity : entities) {
			if (entity->getObject()) {
				dynamicsWorld.removeCollisionObject(entity->getObject());
			}
			delete entity;
		}
	}

	void BulletPhysics::add(EntityReference entity) {	
		entities.push_back(PhysicsEntity::from(entity, &dynamicsWorld));
	}

	void BulletPhysics::remove(EntityReference entity) {
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

}
