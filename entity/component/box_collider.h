#ifndef ENTITY_BOX_COLLIDER_H
#define ENTITY_BOX_COLLIDER_H

#include "entity/component/collider.h"

class BoxCollider : public Collider {
  
public:

  BoxCollider() : Collider(box) {    
  }

  ~BoxCollider() {}

};


#endif //ENTITY_BOX_COLLIDER_H