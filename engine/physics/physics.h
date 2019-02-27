#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "ecs/system.h"

#include "messaging/messaging.h"

#include "entity/manager.h"

namespace wage {

  class Physics : public System, MessageListener<AddEntityMessage>, MessageListener<DestroyEntityMessage> {

  public:
    
    Physics();

    ~Physics();

    LIFECYCLE_FUNC(init)

    virtual void add(Entity entity) = 0;

    virtual void remove(Entity entity) = 0;

    virtual void on(AddEntityMessage& message);
    
    virtual void on(DestroyEntityMessage& message);

  };

}

#endif // PHYSICS_H