#ifndef PHYSICS_ENTITY_H
#define PHYSICS_ENTITY_H

#include <btBulletDynamicsCommon.h>

#include "ecs/registry.h"
#include "physics/rigid_body.h"
#include "physics/collider.h"

#include "physics-bullet/util.h"

namespace wage {
  
  class PhysicsEntity {

  public:

    PhysicsEntity(Entity entity, btCollisionShape* shape, btRigidBody* rigidBody) 
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

    static PhysicsEntity* from(Entity entity, btDiscreteDynamicsWorld* dynamicsWorld);

    PhysicsEntity(Entity entity, btCollisionShape* shape, btRigidBody* rigidBody, btCollisionObject* object)
      : _entity(entity), _shape(shape), _rigidBody(rigidBody), _object(object) {    
    }

    void applyForces();

    void updateEntityTransform();

    void updateShapeTransform();

    inline btRigidBody* rigidBody() {
      return _rigidBody;
    }

    inline btCollisionObject* object() {
      return _object;
    }

    inline btCollisionShape* shape() {
      return _shape;
    }

    inline Entity entity() {
      return _entity;
    }

  private: 

    static btCollisionShape* shapeFor(Entity entity);

    static btRigidBody* rigidBodyFor(Reference<RigidBody> rigidBody, const btTransform& startTransform, btCollisionShape* shape);
    
    Entity _entity;

    btTransform transform(); 

    btCollisionShape* _shape;

    btRigidBody* _rigidBody;

    btCollisionObject* _object;

  };

}

#endif //PHYSICS_ENTITY_H
