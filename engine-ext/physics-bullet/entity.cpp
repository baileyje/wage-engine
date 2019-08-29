#include "physics-bullet/entity.h"

#include "memory/allocator.h"

namespace wage {

  btCollisionShape* PhysicsEntity::shapeFor(Entity entity) {
    if (!entity.has<Collider>()) {
      return make<btEmptyShape>();
    }
    auto scale = entity.get<Transform>()->scale();
    switch (entity.get<Collider>()->type()) {
    // TODO: Support more shapes
    case ColliderType::box: {
      btVector3 halfExtents = btVector3(scale.x / 2.0, scale.y / 2.0, scale.z / 2.0);
      return make<btBoxShape>(halfExtents);
    }
    case ColliderType::sphere: {
      return make<btSphereShape>(scale.x / 2.0);
    }
    default:
      return nullptr;
    }
  }

  btRigidBody* PhysicsEntity::rigidBodyFor(RigidBody* rigidBody, const btTransform& startTransform, btCollisionShape* shape) {
    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
    btVector3 localInertia(0, 0, 0);
    auto mass = rigidBody->mass();
    if (mass != 0.f) {
      shape->calculateLocalInertia(mass, localInertia);
    }
    btDefaultMotionState* myMotionState = make<btDefaultMotionState>(startTransform);
    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
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

  PhysicsEntity* PhysicsEntity::from(Entity entity, btDiscreteDynamicsWorld* dynamicsWorld) {
    btCollisionShape* shape = shapeFor(entity);
    btRigidBody* body = nullptr;
    btCollisionObject* object = nullptr;
    if (entity.has<RigidBody>()) {
      auto entityBody = entity.get<RigidBody>();
      body = rigidBodyFor(entityBody.get(), fromTransform(entity.get<Transform>().get()), shape);
      dynamicsWorld->addRigidBody(body);
      // Must come after added to world.
      if (!entityBody->isAffectedByGravity()) {
        body->setGravity(btVector3(0, 0, 0));
      }
      object = body;
    } else {
      object = make<btCollisionObject>();
      object->setCollisionShape(shape);
      dynamicsWorld->addCollisionObject(object);
    }
    return make<PhysicsEntity>(entity, shape, body, object);
  }

  btTransform PhysicsEntity::transform() {
    btTransform transform;
    if (_rigidBody && _rigidBody->getMotionState()) {
      _rigidBody->getMotionState()->getWorldTransform(transform);
    } else {
      transform = _rigidBody->getWorldTransform();
    }
    return transform;
  }

  void PhysicsEntity::applyForces() {
    if (!_rigidBody || !entity().has<RigidBody>()) {
      return;
    }
    auto entityBody = entity().get<RigidBody>();
    if (entityBody->type() != RigidBodyType::dynamic) {
      return;
    }
    btVector3 impulse = fromVector(entityBody->impulse());
    if (impulse.length() > 0) {
      _rigidBody->applyCentralImpulse(impulse);

      if (_rigidBody->getLinearVelocity().length() > 100) {
        _rigidBody->applyCentralImpulse(-impulse);
      }
    }
    btVector3 force = fromVector(entityBody->force());
    if (force.length() > 0) {
      _rigidBody->applyCentralForce(force);
    }
    btVector3 torque = fromVector(entityBody->torque());
    if (torque.length() > 0) {
      _rigidBody->applyTorque(torque);
    }
    btVector3 torqueImpulse = fromVector(entityBody->torqueImpulse());
    if (torqueImpulse.length() > 0) {
      _rigidBody->applyTorqueImpulse(torqueImpulse);
    }
    if (entityBody->shouldClearLinearVelocity()) {
      _rigidBody->setLinearVelocity(btVector3(0, 0, 0));
    }
    if (entityBody->shouldClearAngularVelocity()) {
      _rigidBody->setAngularVelocity(btVector3(0, 0, 0));
    }
    if (entityBody->shouldStop()) {
      _rigidBody->setLinearVelocity(btVector3(0, 0, 0));
      _rigidBody->setAngularVelocity(btVector3(0, 0, 0));
      entityBody->clearShouldStop();
    }
    entityBody->clearForces();
    _rigidBody->activate(true);
  }

  void PhysicsEntity::updateEntityTransform() {
    if (!_rigidBody || entity().get<RigidBody>()->type() != RigidBodyType::dynamic) {
      return;
    }
    btTransform trans = transform();
    entity().get<Transform>()->position(fromBTVector(trans.getOrigin()));
    btQuaternion rotation = trans.getRotation();
    entity().get<Transform>()->rotation(
        Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w()));
  }

  void PhysicsEntity::updateShapeTransform() {
    if (!_rigidBody) {
      return;
    }
    auto entityBody = entity().get<RigidBody>();
    if (entityBody->type() == RigidBodyType::dynamic) {
      return;
    }
    btTransform transform = fromTransform(entity().get<Transform>().get());
    // _rigidBody->getMotionState()->setWorldTransform(transform);
    _rigidBody->setCenterOfMassTransform(transform);
  }
}