#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include <string>
#include <cinttypes>

#include "math/transform.h"

namespace wage {

  // TODO: Move to network safe ids
  typedef uint32_t ComponentId;

  typedef std::string ComponentName;

  class Component {

  public:

    Component(ComponentName name) : _name(name) {};

    virtual ~Component() {};

    inline ComponentName name() {
      return _name;
    }

  private:

    ComponentName _name;

  };

}

#endif //ENTITY_COMPONENT_H