#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

#include <btBulletDynamicsCommon.h>

class PhysicsEntity {

PhysicsEntity(btTransform transform, btCollisionShape* shape) : transform(transform), shape(shape) {

}

~PhysicsEntity() {
  delete shape;
}

private: 

  btTransform transform;

  btCollisionShape* shape;

  btRigidBody* rigidBody;

};


#endif //PHYSICS_ENTITY_H
