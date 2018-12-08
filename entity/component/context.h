#ifndef COMPONENT_CONTEXT_H
#define COMPONENT_CONTEXT_H

#include "math/transform.h"

namespace wage {

  class Entity;

  class ComponentContext {

  public:

    virtual ~ComponentContext() { }

    virtual Transform& getTransform() = 0;
    
    virtual double getTime() const = 0;

    virtual double getDeltaTime() const = 0;

    virtual Entity* getEntity() = 0;
  };

}


#endif //COMPONENT_CONTEXT_H