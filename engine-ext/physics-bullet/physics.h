#pragma once

#include <btBulletDynamicsCommon.h>

#include <vector>

#include "core/core.h"
#include "ecs/manager.h"
#include "ecs/system.h"
#include "messaging/messaging.h"
#include "physics/physics.h"
#include "physics/collision.h"

#include "physics-bullet/phys_entity.h"

namespace wage {
  namespace physics {

    class BulletPhysics : public Physics {

    public:
      BulletPhysics() : Physics(),
                        dispatcher(&collisionConfiguration),
                        dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration) {}

      virtual ~BulletPhysics() {}

      void start() override {
        Physics::start();
        dynamicsWorld.setInternalTickCallback([](btDynamicsWorld* dynamicsWorld, btScalar timeStep) {
          auto physics = static_cast<BulletPhysics*>(dynamicsWorld->getWorldUserInfo());
          int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
          for (int i = 0; i < numManifolds; i++) {
            auto contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            auto objA = contactManifold->getBody0();
            auto objB = contactManifold->getBody1();
            int numContacts = contactManifold->getNumContacts();
            if (numContacts == 0)
              continue;
            std::vector<ContactPoint> contactsA;
            std::vector<ContactPoint> contactsB;
            auto entityA = static_cast<PhysicsEntity*>(objA->getUserPointer());
            auto entityB = static_cast<PhysicsEntity*>(objB->getUserPointer());
            for (int j = 0; j < numContacts; j++) {
              auto& pt = contactManifold->getContactPoint(j);
              auto& normalOnB = pt.m_normalWorldOnB;
              contactsA.push_back({fromBTVector(pt.getPositionWorldOnA()), fromBTVector(normalOnB)});
              contactsB.push_back({fromBTVector(pt.getPositionWorldOnB()), fromBTVector(normalOnB) * -1});
            }
            physics->addCollision({entityA->entity(), entityB->entity(), contactsA});
            physics->addCollision({entityB->entity(), entityA->entity(), contactsB});
          }
        },
            this);
        // TODO: Add base world config like gravity.
        // dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
        dynamicsWorld.setGravity(btVector3(0, 0, 0));
      }

      void fixedUpdate(const core::Frame& frame) override {
        Physics::fixedUpdate(frame);
        for (auto physicsEntity : entities) {
          if (!physicsEntity->entity().valid()) {
            continue;
          }
          auto entity = physicsEntity->entity();
          if (entity.get<RigidBody>(RigidBodyComponent)->type() == RigidBodyType::dynamic) {
            physicsEntity->applyForces();
          } else {
            physicsEntity->updateShapeTransform();
          }
        }
        // Run the Simulation
        dynamicsWorld.stepSimulation(frame.timeStep(), 3);
        // dynamicsWorld.performDiscreteCollisionDetection();
      }

      virtual std::vector<RayHit> castRay(math::Vector3 from, math::Vector3 to) override {
        btCollisionWorld::AllHitsRayResultCallback allResults(fromVector(from), fromVector(to));
        // allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
        // //kF_UseGjkConvexRaytest flag is now enabled by default, use the faster but more approximate algorithm
        // //allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
        // allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
        dynamicsWorld.rayTest(fromVector(from), fromVector(to), allResults);
        std::vector<RayHit> hits;
        for (int i = 0; i < allResults.m_hitFractions.size(); i++) {
          auto hitPoint = fromBTVector(allResults.m_hitPointWorld[i]);
          auto hitNormal = fromBTVector(allResults.m_hitNormalWorld[i]);
          auto collisionObject = allResults.m_collisionObjects[i];
          auto entity = static_cast<PhysicsEntity*>(collisionObject->getUserPointer());
          hits.push_back({entity->entity(), hitPoint, hitNormal});
        }
        return hits;
      }

      inline void stop() override {
        // TODO: Evaluate memory free, etc.
      }

      inline void add(ecs::Entity entity) override {
        entities.push_back(PhysicsEntity::from(entity, &dynamicsWorld));
      }

      inline void remove(ecs::Entity entity) override {
        for (auto ent = entities.begin(); ent != entities.end(); ++ent) {
          if ((*ent)->entity() == entity) {
            if ((*ent)->rigidBody()) {
              dynamicsWorld.removeRigidBody((*ent)->rigidBody());
            } else {
              dynamicsWorld.removeCollisionObject((*ent)->object());
            }
            entities.erase(ent);
            return;
          }
        }
      }

    private:
      btDefaultCollisionConfiguration collisionConfiguration;

      btCollisionDispatcher dispatcher;

      btDbvtBroadphase overlappingPairCache;

      btSequentialImpulseConstraintSolver solver;

      btDiscreteDynamicsWorld dynamicsWorld;

      std::vector<PhysicsEntity*> entities;
    };

  } // namespace physics
} // namespace wage
