#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include "entity/entity_context.h"

class Component {

public:

  virtual ~Component() {};

  // TODO: Need some kind of enity context
  virtual void update(EntityContext* context) {};

  // TODO: Need some kind of enity context
  virtual void fixedUpdate(EntityContext* context) {};

};

#endif //ENTITY_COMPONENT_H