#ifndef ENTITY_COMPONENT_COLLIDER_H
#define ENTITY_COMPONENT_COLLIDER_H

#include "entity/component.h"

namespace wage {

  enum ColliderType { box, sphere, mesh };

  class Collider : public Component {
    
  public:

    Collider() : Collider(box) {    
    }

    Collider(ColliderType type) : Component("Collider"), type(type) {    
    }

    ~Collider() {}

    inline ColliderType getType() const {
      return type;
    }

    static Collider Box;

    static Collider Sphere;

  private:

    ColliderType type;

  };

}

#endif //ENTITY_COMPONENT_COLLIDER_H