#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include <typeinfo>

#include "entity/entity_context.h"

class Component {

public:

  Component(std::string name) : name(name) {};

  virtual ~Component() {};

  // TODO: Need some kind of enity context
  virtual void update(EntityContext* context) {};

  // TODO: Need some kind of enity context
  virtual void fixedUpdate(EntityContext* context) {};

  inline std::string getName() {
    return name;
  }

private:

  std::string name;
};

#endif //ENTITY_COMPONENT_H