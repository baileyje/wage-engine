#include "physics/physics.h"

#include "physics/rigid_body.h"
#include "physics/collider.h"
#include "core/logger.h"

// #include "ecs/system_context.h"

namespace wage {

	Physics::Physics() : Service("Physics") {}

	Physics::~Physics() {}

	void Physics::start() {
		Core::Instance->get<Messaging>()->listen<AddEntityMessage>(this);
		Core::Instance->get<Messaging>()->listen<DestroyEntityMessage>(this);
	}

	void Physics::on(AddEntityMessage& message) {
		auto entity = message.entity();
		if (
			(entity.has<RigidBody>() && entity.get<RigidBody>().valid()) ||
			(entity.has<Collider>() && entity.get<Collider>().valid())
			) {
			add(entity);
		}		
	}

	void Physics::on(DestroyEntityMessage& message) {
		remove(message.entity());
	}

}
