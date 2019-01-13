#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

#include <btBulletDynamicsCommon.h>

#include "entity/reference.h"
#include "physics/rigid_body.h"
#include "physics/collider.h"

#include "physics-bullet/util.h"

namespace wage {
  
  class PhysicsEntity {

  public:

    PhysicsEntity(EntityReference entity, btCollisionShape* shape, btRigidBody* rigidBody) 
      : PhysicsEntity(entity, shape, rigidBody, rigidBody) {
    }

    ~PhysicsEntity() {    
      // TODO: Evaluate memory usage. Assume these are lost.
      // if (rigidBody && rigidBody->getMotionState()) {
      //   delete rigidBody->getMotionState();
      // }      
      // delete object; // NOT rigidBody
      // delete shape;
    }  

    static PhysicsEntity* from(EntityReference entity, btDiscreteDynamicsWorld* dynamicsWorld);

    PhysicsEntity(EntityReference entity, btCollisionShape* shape, btRigidBody* rigidBody, btCollisionObject* object)
      : entity_(entity), shape_(shape), rigidBody_(rigidBody), object_(object) {    
    }

    void applyForces();

    void updateEntityTransform();

    void updateShapeTransform();

    inline btRigidBody* rigidBody() {
      return rigidBody_;
    }

    inline btCollisionObject* object() {
      return object_;
    }

    inline EntityReference entity() {
      return entity_;
    }

  private: 

    static btCollisionShape* shapeFor(EntityReference entity);

    static btRigidBody* rigidBodyFor(ComponentReference<RigidBody> rigidBody, const btTransform& startTransform, btCollisionShape* shape);
    
    EntityReference entity_;

    btTransform transform(); 

    btCollisionShape* shape_;

    btRigidBody* rigidBody_;

    btCollisionObject* object_;

  };

}

#endif //PHYSICS_ENTITY_H
