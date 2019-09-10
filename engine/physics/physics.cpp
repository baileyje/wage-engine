#include "physics/physics.h"

#include "physics/rigid_body.h"
#include "physics/collider.h"
#include "core/logger.h"

namespace wage { namespace physics {

  Physics::Physics() : Service("Physics") {}

  Physics::~Physics() {}

  void Physics::start() {
    core::Core::Instance->get<messaging::Messaging>()->listen<ecs::AddEntityMessage>(this);
    core::Core::Instance->get<messaging::Messaging>()->listen<ecs::DestroyEntityMessage>(this);
  }

  bool Physics::on(const ecs::AddEntityMessage& message) {
    auto entity = message.entity();
    if (
        (entity.has<RigidBody>()) || (entity.has<Collider>())) {
      add(entity);
    }
    return false;
  }

  bool Physics::on(const ecs::DestroyEntityMessage& message) {
    remove(message.entity());
    return false;
  }

} }
