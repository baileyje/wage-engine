#ifndef ENTITY_RIGID_BODY_H
#define ENTITY_RIGID_BODY_H

#include "entity/component.h"
#include "entity/vector.h"

class RigidBody : public Component {

public:

  RigidBody() : Component("RigidBody"), mass(0), affectedByGravity(true) {}

  ~RigidBody() {}

  float mass;

  Vector velocity;

  Vector acceleration;

  bool affectedByGravity;

  Vector currentForce;

  void addForce(Vector impulse) {
    currentForce += impulse;
  }

private:
  
  

};

#endif //ENTITY_RIGID_BODY_H
