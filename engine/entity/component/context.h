#ifndef COMPONENT_CONTEXT_H
#define COMPONENT_CONTEXT_H

#include "math/transform.h"
#include "entity/reference.h"

namespace wage {

  class ComponentContext {

  public:

    virtual ~ComponentContext() { }

    virtual Transform& transform() = 0;
    
    virtual double time() const = 0;

    virtual double deltaTime() const = 0;

    virtual EntityReference entity() = 0;

  };

}


#endif //COMPONENT_CONTEXT_H