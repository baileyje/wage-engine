#pragma once

#include "new_ecs/entity.h"

namespace wage {
  namespace ecs {

    class EntityMessage {

    public:
      EntityMessage(Entity entity) : _entity(entity) {}

      virtual ~EntityMessage() {}

      inline Entity entity() const {
        return _entity;
      }

    private:
      Entity _entity;
    };

    class AddEntityMessage : public EntityMessage {

    public:
      AddEntityMessage(Entity entity) : EntityMessage(entity) {}
    };

    class DestroyEntityMessage : public EntityMessage {

    public:
      DestroyEntityMessage(Entity entity) : EntityMessage(entity) {}
    };
  }
}