#include "physics-bullet/entity.h"

#include "memory/allocator.h"

namespace wage {

  btCollisionShape* PhysicsEntity::shapeFor(EntityReference entity) {
    auto collider = entity.get<Collider>();
    if (!collider.valid()) {
      return make<btEmptyShape>();
    }
    Vector scale = entity->getTransform().getScale();  
    switch(collider->getType()) {
      // TODO: Support more shapes
      case box: {
        btVector3 halfExtents = btVector3(scale.x / 2.0, scale.y / 2.0, scale.z / 2.0);			
        return make<btBoxShape>(halfExtents);
      }
      case sphere: {
        return make<btSphereShape>(scale.x / 2.0);
      }
      default: return nullptr;
    }	
  }

  btRigidBody* PhysicsEntity::rigidBodyFor(ComponentReference<RigidBody> rigidBody, const btTransform& startTransform, btCollisionShape* shape) {
    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
    btVector3 localInertia(0, 0, 0);    
    if (rigidBody->getMass() != 0.f) {      
      shape->calculateLocalInertia(rigidBody->getMass(), localInertia);
    }
    btDefaultMotionState* myMotionState = make<btDefaultMotionState>(startTransform);
    btRigidBody::btRigidBodyConstructionInfo cInfo(rigidBody->getMass(), myMotionState, shape, localInertia);
    btRigidBody* body = make<btRigidBody>(cInfo);
    if (rigidBody->getType() == kinematic) {
      body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    } else if (rigidBody->getType() == immovable) {
      body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    }
    //body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);    
    body->setUserIndex(-1);
    return body;
  }

  PhysicsEntity* PhysicsEntity::from(EntityReference entity, btDiscreteDynamicsWorld* dynamicsWorld) {
    btCollisionShape* shape = shapeFor(entity);
    btRigidBody* body = nullptr;
    btCollisionObject* object = nullptr;
    auto entityBody = entity.get<RigidBody>();
    if (entityBody.valid()) {
      body = rigidBodyFor(entityBody, fromTransform(entity->getTransform()), shape);
      dynamicsWorld->addRigidBody(body);
      // Must come after added to world.    
      if (!entityBody->isAffectedByGravity()) {
        body->setGravity(btVector3(0, 0, 0));
      }
      object = body;
    }	else {      
      object = make<btCollisionObject>();
      object->setCollisionShape(shape);
      dynamicsWorld->addCollisionObject(object);
    }
    return make<PhysicsEntity>(entity, shape, body, object);
  }

  btTransform PhysicsEntity::getTransform() {
    btTransform transform;
    if (rigidBody && rigidBody->getMotionState()) {
      rigidBody->getMotionState()->getWorldTransform(transform);
      
    } else {  
      transform = rigidBody->getWorldTransform();
    }
    return transform;
  }

  void PhysicsEntity::applyForces() {
    if (!rigidBody) {
      return;
    }
    auto entityBody = entity.get<RigidBody>();
    if (!entityBody.valid() || entityBody->getType() != dynamic) {
      return;
    }
    btVector3 impulse = fromVector(entityBody->getImpulse());
    if (impulse.length() > 0) {
      rigidBody->applyCentralImpulse(impulse);	
      entityBody->clearImpulse();
    }
    btVector3 force = fromVector(entityBody->getForce());
    if (force.length() > 0) {
      rigidBody->applyCentralForce(force);	
    }			
    rigidBody->activate(true);
  }

  void PhysicsEntity::updateEntityTransform() {
    if (!rigidBody) {
      return;
    }
    btTransform transform = getTransform();  
    entity->getTransform().setPosition(fromBTVector(transform.getOrigin()));
    btQuaternion rotation = transform.getRotation();
    // btScalar yawZ, pitchY, rollX;
    // rotation.getEulerZYX(yawZ, pitchY, rollX);
    entity->getTransform().setRotation(
      Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z())
    );
  }

  void PhysicsEntity::updateShapeTransform() {
    if (!rigidBody) {
      return;
    }
    auto entityBody = entity.get<RigidBody>();
    if (entityBody->getType() == dynamic) {
      return;
    }  
    btTransform transform = fromTransform(entity->getTransform());
    rigidBody->getMotionState()->setWorldTransform(transform);
  }

}