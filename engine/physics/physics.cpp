#include "physics/physics.h"

#include "physics/rigid_body.h"
#include "physics/collider.h"
#include "core/logger.h"
#include "ecs/system_context.h"

namespace wage {

	Physics::Physics() : System("Physics") {}

	Physics::~Physics() {}

	void Physics::init(SystemContext* context) {
		Core::Instance->get<Messaging>()->listen<AddEntityMessage>(this);
		Core::Instance->get<Messaging>()->listen<DestroyEntityMessage>(this);
	}

	void Physics::on(AddEntityMessage& message) {
		auto entity = message.entity();
		if (entity.has<RigidBody>() && entity.get<RigidBody>().valid()) {
			add(entity);
		}		
	}

	void Physics::on(DestroyEntityMessage& message) {
		remove(message.entity());
	}

}
