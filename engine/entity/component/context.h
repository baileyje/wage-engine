#ifndef COMPONENT_CONTEXT_H
#define COMPONENT_CONTEXT_H

#include "engine/math/transform.h"
#include "engine/entity/reference.h"

namespace wage {

  class ComponentContext {

  public:

    virtual ~ComponentContext() { }

    virtual Transform& getTransform() = 0;
    
    virtual double getTime() const = 0;

    virtual double getDeltaTime() const = 0;

    virtual EntityReference getEntity() = 0;

  };

}


#endif //COMPONENT_CONTEXT_H