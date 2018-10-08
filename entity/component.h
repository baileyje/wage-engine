#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include "core/context.h"

class Component {

public:

  virtual ~Component() {};

  virtual void update(Context* context) {};

  virtual void fixedUpdate(Context* context) {};

};

#endif //ENTITY_COMPONENT_H