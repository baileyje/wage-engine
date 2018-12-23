#include "physics/physics.h"

#include "physics/rigid_body.h"
#include "physics/collider.h"
#include "core/logger.h"
#include "core/system/context.h"

namespace wage {

	Physics::Physics() : System("Physics") {}

	Physics::~Physics() {}

	void Physics::init(SystemContext* context) {
		context->get<Messaging>()->listen<AddEntityMessage>(this);
		context->get<Messaging>()->listen<DestroyEntityMessage>(this);
	}

	void Physics::on(AddEntityMessage& message) {
		auto entity = message.getEntity();
		if (entity->get<RigidBody>() != nullptr) {
			add(entity);
		}		
	}

	void Physics::on(DestroyEntityMessage& message) {
		remove(message.getEntity());
	}

}
