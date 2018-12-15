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

    virtual Transform* getTransform() {
      return transform;
    }

    virtual void setTransform(Transform* transform) {
      this->transform = transform;
    }

  private:

    std::string name;

    Transform* transform;
    
  };

}

#endif //ENTITY_COMPONENT_H