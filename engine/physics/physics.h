#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "core/service.h"
#include "ecs/entity_manager.h"

#include "messaging/messaging.h"


namespace wage {

  class Physics : public Service, MessageListener<AddEntityMessage>, MessageListener<DestroyEntityMessage> {

  public:
    
    Physics();

    ~Physics();

    virtual void start() override;

    virtual void add(Entity entity) = 0;

    virtual void remove(Entity entity) = 0;

    virtual void on(AddEntityMessage& message) override;
    
    virtual void on(DestroyEntityMessage& message) override ;

  };

}

#endif // PHYSICS_H