#include "physics-bullet/entity.h"

#include "memory/allocator.h"

namespace wage {

  btCollisionShape* PhysicsEntity::shapeFor(EntityReference entity) {
    auto collider = entity.get<Collider>();
    if (!collider.valid()) {
      return make<btEmptyShape>();
    }
    Vector scale = entity->transform().scale();  
    switch(collider->type()) {
      // TODO: Support more shapes
      case ColliderType::box: {
        btVector3 halfExtents = btVector3(scale.x / 2.0, scale.y / 2.0, scale.z / 2.0);			
        return make<btBoxShape>(halfExtents);
      }
      case ColliderType::sphere: {
        return make<btSphereShape>(scale.x / 2.0);
      }
      default: return nullptr;
    }	
  }

  btRigidBody* PhysicsEntity::rigidBodyFor(ComponentReference<RigidBody> rigidBody, const btTransform& startTransform, btCollisionShape* shape) {
    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
    btVector3 localInertia(0, 0, 0);    
    if (rigidBody->mass() != 0.f) {      
      shape->calculateLocalInertia(rigidBody->mass(), localInertia);
    }
    btDefaultMotionState* myMotionState = make<btDefaultMotionState>(startTransform);
    btRigidBody::btRigidBodyConstructionInfo cInfo(rigidBody->mass(), myMotionState, shape, localInertia);
    btRigidBody* body = make<btRigidBody>(cInfo);
    if (rigidBody->type() == RigidBodyType::kinematic) {
      body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    } else if (rigidBody->type() == RigidBodyType::immovable) {
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
      body = rigidBodyFor(entityBody, fromTransform(entity->transform()), shape);
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

  btTransform PhysicsEntity::transform() {
    btTransform transform;
    if (rigidBody_ && rigidBody_->getMotionState()) {
      rigidBody_->getMotionState()->getWorldTransform(transform);
      
    } else {  
      transform = rigidBody_->getWorldTransform();
    }
    return transform;
  }

  void PhysicsEntity::applyForces() {
    if (!rigidBody_) {
      return;
    }
    auto entityBody = entity().get<RigidBody>();
    if (!entityBody.valid() || entityBody->type() != RigidBodyType::dynamic) {
      return;
    }
    btVector3 impulse = fromVector(entityBody->impulse());
    if (impulse.length() > 0) {
      rigidBody_->applyCentralImpulse(impulse);	
      entityBody->clearImpulse();
    }
    btVector3 force = fromVector(entityBody->force());
    if (force.length() > 0) {
      rigidBody_->applyCentralForce(force);	
    }			
    rigidBody_->activate(true);
  }

  void PhysicsEntity::updateEntityTransform() {
    if (!rigidBody_) {
      return;
    }
    btTransform trans = transform();  
    entity()->transform().setPosition(fromBTVector(trans.getOrigin()));
    btQuaternion rotation = trans.getRotation();
    // btScalar yawZ, pitchY, rollX;
    // rotation.getEulerZYX(yawZ, pitchY, rollX);
    entity()->transform().setRotation(
      Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z())
    );
  }

  void PhysicsEntity::updateShapeTransform() {
    if (!rigidBody_) {
      return;
    }
    auto entityBody = entity().get<RigidBody>();
    if (entityBody->type() == RigidBodyType::dynamic) {
      return;
    }  
    btTransform transform = fromTransform(entity()->transform());
    rigidBody_->getMotionState()->setWorldTransform(transform);
  }

}