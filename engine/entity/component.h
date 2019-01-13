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

    Component(ComponentName name) : name_(name) {};

    virtual ~Component() {};

    inline ComponentName name() {
      return name_;
    }

    virtual bool isDynamic() {
      return false;
    }

    virtual Transform* transform() {
      return transform_;
    }

    virtual void setTransform(Transform* transform) {
      transform_ = transform;
    }

  private:

    ComponentName name_;

    Transform* transform_;
    
  };

}

#endif //ENTITY_COMPONENT_H