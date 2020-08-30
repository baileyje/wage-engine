#pragma once

#include <btBulletDynamicsCommon.h>

#include <vector>

#include "ecs/system.h"
#include "messaging/messaging.h"
#include "physics/physics.h"

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
        // dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
        dynamicsWorld.setGravity(btVector3(0, 0, 0));
        core::Core::Instance->onFixedUpdate([&](const core::Frame& frame) {
          fixedUpdate(frame);
        });
      }

      void fixedUpdate(const core::Frame& frame) {
        // Get Physics up to speed
        for (auto physicsEntity : entities) {
          if (!physicsEntity->entity().valid()) {
            continue;
          }
          physicsEntity->updateShapeTransform();
          physicsEntity->applyForces();
        }
        // Run the Simulation
        dynamicsWorld.stepSimulation(frame.timeStep(), 3);
        // dynamicsWorld.performDiscreteCollisionDetection();
      }

      inline void stop() override {
        // TODO: Evaluate memory free, etc.
      }

      inline void add(ecs::Entity entity) override {
        entities.push_back(PhysicsEntity::from(entity, &dynamicsWorld));
      }

      inline void remove(ecs::Entity entity) override {
        for (auto ent = entities.begin(); ent != entities.end(); ++ent) {
          // TODO: Remove entity stuff from world
          //      - rigidBody
          //      - collisionObject

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

  }
}