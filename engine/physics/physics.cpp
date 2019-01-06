#include "engine/physics/physics.h"

#include "engine/physics/rigid_body.h"
#include "engine/physics/collider.h"
#include "engine/core/logger.h"
#include "engine/core/system/context.h"

namespace wage {

	Physics::Physics() : System("Physics") {}

	Physics::~Physics() {}

	void Physics::init(SystemContext* context) {
		context->get<Messaging>()->listen<AddEntityMessage>(this);
		context->get<Messaging>()->listen<DestroyEntityMessage>(this);
	}

	void Physics::on(AddEntityMessage& message) {
		auto entity = message.getEntity();
		if (entity.get<RigidBody>().valid()) {
			add(entity);
		}		
	}

	void Physics::on(DestroyEntityMessage& message) {
		remove(message.getEntity());
	}

}
