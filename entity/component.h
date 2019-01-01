#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include <string>

#include "math/transform.h"

namespace wage {

  // TODO: Move to network safe ids
  typedef unsigned long ComponentId;

  typedef std::string ComponentName;

  class Component {

  public:

    Component(ComponentName name) : name(name) {};

    virtual ~Component() {};

    inline ComponentName getName() {
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

    ComponentName name;

    Transform* transform;
    
  };

}

#endif //ENTITY_COMPONENT_H