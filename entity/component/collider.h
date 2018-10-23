#ifndef ENTITY_COMPONENT_COLLIDER_H
#define ENTITY_COMPONENT_COLLIDER_H

#include "entity/component.h"

enum ColliderType { box, mesh };

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

private:

  ColliderType type;

};


#endif //ENTITY_COMPONENT_COLLIDER_H