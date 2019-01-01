#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

#include <btBulletDynamicsCommon.h>

#include "entity/manager.h" // EntityReference :(
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
      : entity(entity), shape(shape), rigidBody(rigidBody), object(object) {    
    }

    void applyForces();

    void updateEntityTransform();

    void updateShapeTransform();

    inline btRigidBody* getRigidBody() {
      return rigidBody;
    }

    inline btCollisionObject* getObject() {
      return object;
    }

    inline EntityReference getEntity() {
      return entity;
    }

  private: 

    static btCollisionShape* shapeFor(EntityReference entity);

    static btRigidBody* rigidBodyFor(ComponentReference<RigidBody> rigidBody, const btTransform& startTransform, btCollisionShape* shape);
    
    EntityReference entity;

    btTransform getTransform(); 

    btCollisionShape* shape;

    btRigidBody* rigidBody;

    btCollisionObject* object;

  };

}

#endif //PHYSICS_ENTITY_H
