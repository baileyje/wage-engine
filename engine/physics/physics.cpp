#include "physics/physics.h"

#include "physics/rigid_body.h"
#include "physics/collider.h"
#include "core/logger.h"

namespace wage {

  Physics::Physics() : Service("Physics") {}

  Physics::~Physics() {}

  void Physics::start() {
    Core::Instance->get<Messaging>()->listen<AddEntityMessage>(this);
    Core::Instance->get<Messaging>()->listen<DestroyEntityMessage>(this);
  }

  bool Physics::on(const AddEntityMessage& message) {
    auto entity = message.entity();
    if (
        (entity.has<RigidBody>() && entity.get<RigidBody>().valid()) ||
        (entity.has<Collider>() && entity.get<Collider>().valid())) {
      add(entity);
    }
    return false;
  }

  bool Physics::on(const DestroyEntityMessage& message) {
    remove(message.entity());
    return false;
  }
}
