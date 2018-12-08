#ifndef DYNAMIC_COMPONENT_H
#define DYNAMIC_COMPONENT_H

#include "entity/component.h"

namespace wage {

  class DynamicComponent : public Component {

  public:
    
    DynamicComponent(std::string name) : Component(name) {};

    virtual ~DynamicComponent() {};

    virtual void start(ComponentContext* context);

    virtual void update(ComponentContext* context);

    virtual void fixedUpdate(ComponentContext* context);

    virtual void stop(ComponentContext* context);

    virtual bool isDynamic() {
      return true;
    }
  };

}

#endif //DYNAMIC_COMPONENT_H