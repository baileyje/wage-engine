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

    Component(ComponentName name) : _name(name) {};

    virtual ~Component() {};

    inline ComponentName name() {
      return _name;
    }

    virtual bool isDynamic() {
      return false;
    }

    virtual Transform* transform() {
      return _transform;
    }

    virtual void transform(Transform* transform) {
      _transform = transform;
    }

  private:

    ComponentName _name;

    Transform* _transform;
    
  };

}

#endif //ENTITY_COMPONENT_H