#pragma once

#include <btBulletDynamicsCommon.h>

#include "ecs/registry.h"
#include "ecs/entity.h"
#include "physics/components/rigid_body.h"
#include "physics/components/collider.h"

#include "physics-bullet/util.h"

namespace wage {
  namespace physics {

    class PhysicsEntity : public btMotionState {

    public:
      static PhysicsEntity* from(ecs::Entity entity, btDiscreteDynamicsWorld* dynamicsWorld) {
        auto physEntity = memory::make<PhysicsEntity>(entity);
        dynamicsWorld->addRigidBody(physEntity->_rigidBody);
        return physEntity;
      }

      PhysicsEntity(ecs::Entity entity) : _entity(entity) {
        auto entTransform = entity.get<math::Transform>(TransformComponent);
        centerOfMassOffset = btTransform::getIdentity();
        transform = fromTransform(entTransform);
        auto shape = shapeFor(entity);
        auto rigidBody = entity.get<RigidBody>(RigidBodyComponent);
        btVector3 localInertia(0, 0, 0);
        auto mass = rigidBody->mass();
        if (mass != 0.f) {
          shape->calculateLocalInertia(mass, localInertia);
        }
        btRigidBody::btRigidBodyConstructionInfo cInfo(mass, this, shape, localInertia);
        _rigidBody = memory::make<btRigidBody>(cInfo);
        if (rigidBody->type() == RigidBodyType::kinematic) {
          _rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        } else if (rigidBody->type() == RigidBodyType::immovable) {
          _rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        }
        //body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
        _rigidBody->setUserIndex(entity.id().id());
        _rigidBody->setUserPointer(this);
      }

      ~PhysicsEntity() {
        // TODO: Evaluate memory usage. Assume these are lost.
        // if (rigidBody && rigidBody->getMotionState()) {
        //   delete rigidBody->getMotionState();
        // }
        // delete object; // NOT rigidBody
        // delete shape;
      }

      void applyForces() {
        if (!_rigidBody || !entity().has(RigidBodyComponent)) {
          return;
        }
        auto entityBody = entity().get<RigidBody>(RigidBodyComponent);
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

      void updateShapeTransform() {
        if (!_rigidBody) {
          return;
        }
        auto entityBody = entity().get<RigidBody>(RigidBodyComponent);
        if (entityBody->type() == RigidBodyType::dynamic) {
          return;
        }
        transform = fromTransform(entity().get<math::Transform>(TransformComponent));
        // _rigidBody->getMotionState()->setWorldTransform(transform);
        _rigidBody->setCenterOfMassTransform(transform);
      }

      inline ecs::Entity entity() {
        return _entity;
      }

      inline btRigidBody* rigidBody() {
        return _rigidBody;
      }

      inline btCollisionObject* object() {
        return _rigidBody;
      }

      virtual void getWorldTransform(btTransform& ret) const {
        ret = transform;
      }

      virtual void setWorldTransform(const btTransform& in) {
        if (!_entity.valid())
          return;
        transform = in;
        btTransform transform = in * centerOfMassOffset;
        auto entityTransform = entity().get<math::Transform>(TransformComponent);
        entityTransform->localPosition = fromBTVector(transform.getOrigin());
        btQuaternion rotation = transform.getRotation();
        entityTransform->localRotation = math::Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w());
      }

    private:
      static btCollisionShape* shapeFor(ecs::Entity entity);

      static btRigidBody* rigidBodyFor(RigidBody* rigidBody, const btTransform& startTransform, btCollisionShape* shape);

      ecs::Entity _entity;

      btCollisionShape* _shape;

      btRigidBody* _rigidBody;

      btTransform transform;

      btTransform centerOfMassOffset;
    };
  }
}
