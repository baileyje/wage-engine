#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include <string>

#include "entity/component/context.h"

class Component {

public:

  Component(std::string name) : name(name) {};

  virtual ~Component() {};

  virtual void start(ComponentContext* context);

  virtual void update(ComponentContext* context);

  virtual void fixedUpdate(ComponentContext* context);

  virtual void stop(ComponentContext* context);

  inline std::string getName() {
    return name;
  }

private:

  std::string name;
};

#endif //ENTITY_COMPONENT_H