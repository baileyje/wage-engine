#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include <string>

#include "entity/component/context.h"

namespace wage {

  class Component {

  public:

    Component(std::string name) : name(name) {};

    virtual ~Component() {};

    inline std::string getName() {
      return name;
    }

    virtual bool isDynamic() {
      return false;
    }

  private:

    std::string name;

  };

}

#endif //ENTITY_COMPONENT_H