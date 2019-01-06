#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "engine/core/system.h"

#include "engine/messaging/messaging.h"

#include "engine/entity/manager.h"

namespace wage {

  class Physics : public System, MessageListener<AddEntityMessage>, MessageListener<DestroyEntityMessage> {

  public:
    
    Physics();

    ~Physics();

    LIFECYCLE_FUNC(init)

    virtual void add(EntityReference entity) = 0;

    virtual void remove(EntityReference entity) = 0;

    virtual void on(AddEntityMessage& message);
    
    virtual void on(DestroyEntityMessage& message);

  };

}

#endif // PHYSICS_H