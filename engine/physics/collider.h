#ifndef ENTITY_COMPONENT_COLLIDER_H
#define ENTITY_COMPONENT_COLLIDER_H

#include "entity/component.h"

namespace wage {

  enum class ColliderType { box, sphere, mesh };

  class Collider : public Component {
    
  public:

    Collider() : Collider(ColliderType::box) {    
    }

    Collider(ColliderType type) : Component("Collider"), type_(type) {    
    }

    ~Collider() {}

    inline ColliderType type() const {
      return type_;
    }

    static Collider Box;

    static Collider Sphere;

  private:

    ColliderType type_;

  };

}

#endif //ENTITY_COMPONENT_COLLIDER_H