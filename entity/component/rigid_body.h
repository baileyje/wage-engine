#ifndef ENTITY_RIGID_BODY_H
#define ENTITY_RIGID_BODY_H

#include "entity/component.h"
#include "entity/vector.h"

class RigidBody : public Component {

public:

  RigidBody() : RigidBody(0) {    
  }

  RigidBody(float mass) : Component("RigidBody"), mass(mass), affectedByGravity(true) {}

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
