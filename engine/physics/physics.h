#pragma once

#include <vector>

#include "core/service.h"
#include "ecs/entity_manager.h"

#include "messaging/messaging.h"

namespace wage { namespace physics {

  class Physics : public core::Service, messaging::MessageListener<ecs::AddEntityMessage>, messaging::MessageListener<ecs::DestroyEntityMessage> {

  public:
    Physics();

    ~Physics();

    virtual void start() override;

    virtual void add(Entity entity) = 0;

    virtual void remove(Entity entity) = 0;

    virtual bool on(const ecs::AddEntityMessage& message) override;

    virtual bool on(const ecs::DestroyEntityMessage& message) override;
  };

} }