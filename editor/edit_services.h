#pragma once

#include <vector>

#include <btBulletDynamicsCommon.h>

#include "engine.h"
#include "gui.h"

namespace wage::editor {

  class EditorController : public core::Service, public messaging::MessageListener<input::KeyEvent> {
  public:
    EditorController() : Service("EditorController") {}

    void start() {
      core::Core::Instance->get<messaging::Messaging>()->listen<input::KeyEvent>(this);
    }

    inline bool on(const input::KeyEvent& event) {

      return false;
    }

  private:
  };

  class EditorGizmos : public core::Service {
  public:
    EditorGizmos() : Service("EditorGizmos") {}

    void start() {
      core::Core::Instance->onUpdate([&](const core::Frame& frame) {
        auto& entities = scene::Scene::current().entities();
        auto renderer = core::Core::Instance->get<render::Renderer>();
        for (auto entity : entities.with({ColliderComponent, TransformComponent})) {
          if (!entity.valid()) {
            continue;
          }
          auto transform = entity.get<math::Transform>(TransformComponent);
          auto collider = entity.get<physics::Collider>(ColliderComponent);
          // TODO: FIX THIS BEFORE IT GOES ANY FURTHER AND KILLS SOMEONE.
          math::Transform colliderRenderTransform(collider->transform.worldProjection(*transform));
          if (collider->type == physics::Collider::Type::box)
            renderer->renderWireframe(colliderRenderTransform, {"cube.obj"});
          else if (collider->type == physics::Collider::Type::sphere)
            renderer->renderWireframe(colliderRenderTransform, {render::MeshSpec::Sphere});
        }
        for (auto entity : entities.with({TransformComponent})) {
          if (entity.has(PerspectiveCameraComponent)) continue;
          auto transform = entity.get<math::Transform>(TransformComponent);
          auto scale = transform->scale();
          math::Transform forwardHandleTransform(math::Transform({0, 0, 2 / scale.z}, {.1f / scale.x, .1f / scale.y, 4.0f / scale.z}, math::Quaternion()).worldProjection(*transform));
          renderer->renderMesh(forwardHandleTransform, {"cube.obj"}, {{"blue.png"}});

          math::Transform upHandleTransform(math::Transform({0, 2 / scale.y, 0}, {.1f / scale.x, 4.0f / scale.y, .1f / scale.z}, math::Quaternion()).worldProjection(*transform));
          renderer->renderMesh(upHandleTransform, {"cube.obj"}, {{"green.png"}});

          math::Transform rightHandleTransform(math::Transform({-2 / scale.x, 0, 0}, {4.0f / scale.x, .1f / scale.y, .1f / scale.z}, math::Quaternion()).worldProjection(*transform));
          renderer->renderMesh(rightHandleTransform, {"cube.obj"}, {{"red.png"}});
        }
      });
    }

  private:
  };

  struct EntityHolder {
    ecs::Entity entity;
    btCollisionObject* collisionObject;
  };

  class EditorSelectionController : public core::Service, public messaging::MessageListener<input::KeyEvent>, public messaging::MessageListener<input::MouseButtonEvent>, messaging::MessageListener<ecs::AddEntityMessage>, messaging::MessageListener<ecs::DestroyEntityMessage> {
  public:
    EditorSelectionController() : Service("EditorSelectionController"), dispatcher(&collisionConfiguration), dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfiguration) {}

    void start() {
      core::Core::Instance->get<messaging::Messaging>()->listen<input::KeyEvent>(this);
      core::Core::Instance->get<messaging::Messaging>()->listen<input::MouseButtonEvent>(this);
      core::Core::Instance->get<messaging::Messaging>()->listen<ecs::AddEntityMessage>(this);
      core::Core::Instance->get<messaging::Messaging>()->listen<ecs::DestroyEntityMessage>(this);

      core::Core::Instance->onUpdate([&](const core::Frame& frame) {
        for (auto entity : entities) {
          auto transform = entity.entity.get<math::Transform>(TransformComponent);
          entity.collisionObject->setWorldTransform(fromTransform(transform));
        }
      });
    }

    inline bool on(const ecs::AddEntityMessage& message) {
      auto entity = message.entity();
      if (!entity.has(TransformComponent)) return false;
      entities.push_back({entity, new btCollisionObject() });
      auto ref = &entities.back();
      auto transform = entity.get<math::Transform>(TransformComponent);
      auto scale = transform->localScale;
      
      // TODO: Scale this by the entity and make sure it stays up to date.....  This doesn't handle resize..
      ref->collisionObject->setWorldTransform(fromTransform(transform));
      ref->collisionObject->setCollisionShape(new btSphereShape(1));
      ref->collisionObject->setCollisionFlags(ref->collisionObject->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
      ref->collisionObject->setUserPointer(ref);
      dynamicsWorld.addCollisionObject(ref->collisionObject);

      return false;
    }

    inline bool on(const ecs::DestroyEntityMessage& message) {
      return false;
    }

    inline bool on(const input::KeyEvent& event) {

      return false;
    }

    inline bool on(const input::MouseButtonEvent& event) {
      
      auto position = event.position();
      auto rayPosition = getRayFromScreenSpace(position);
      
      auto entities = &scene::Scene::current().entities();
      auto cAndE = cameraAndEntity(entities);
      auto cameraEntity = std::get<0>(cAndE);
      auto camTransform = cameraEntity.get<math::Transform>(TransformComponent);

      castRay(camTransform->position() + rayPosition, camTransform->position() + rayPosition * 1000);

      return false;
    }

  private:
    inline btVector3 fromVector(math::Vector vector) {
      return btVector3(vector.x, vector.y, vector.z);
    }

    inline math::Vector fromBTVector(btVector3 vector) {
      return math::Vector(vector.x(), vector.y(), vector.z());
    }

    inline btTransform fromTransform(math::Transform* transform) {
      btTransform btTransform;
      btTransform.setIdentity();
      btTransform.setOrigin(fromVector(transform->localPosition));
      math::Quaternion transformRotation = transform->localRotation;
      btQuaternion rotation(transformRotation.x, transformRotation.y, transformRotation.z, transformRotation.w);
      btTransform.setRotation(rotation);
      return btTransform;
    }

    math::Vector3 getRayFromScreenSpace(math::Vector2 pos) {

      auto entities = &scene::Scene::current().entities();
      auto cAndE = cameraAndEntity(entities);
      auto cameraEntity = std::get<0>(cAndE);
      auto camera = std::get<1>(cAndE);
      auto platform = core::Core::Instance->get<platform::Platform>();
      auto window = platform->window();
      auto screenProjection = camera->screenProjection({ static_cast<float>(window->width()), static_cast<float>(window->height()) });
      auto camTransform = cameraEntity.get<math::Transform>(TransformComponent);
      auto viewProjection = camera->viewProjection(camTransform);

      auto x = (2.0f * pos.x ) / window->width() - 1.0f;
      auto y = 1.0f - (2.0f * pos.y) / window->height();
      float z = -1.0f;
      glm::vec3 radDevCoords = glm::vec3(x, y, z);
      glm::vec4 rayClip = glm::vec4(radDevCoords, 1.0);
      glm::vec4 rayEye = glm::inverse(screenProjection.glm()) * rayClip;
      rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
      glm::vec3 rayWorld = glm::vec3(glm::inverse(viewProjection.glm()) * rayEye);
      rayWorld = glm::normalize(rayWorld);
      // Why the z flip...
      return { rayWorld.x, rayWorld.y, rayWorld.z };
    }

     void castRay(math::Vector3 from, math::Vector3 to) {
      btCollisionWorld::AllHitsRayResultCallback allResults(fromVector(from), fromVector(to));
      dynamicsWorld.rayTest(fromVector(from), fromVector(to), allResults);
      for (int i = 0; i < allResults.m_hitFractions.size(); i++) {
        auto hitPoint = fromBTVector(allResults.m_hitPointWorld[i]);
        auto hitNormal = fromBTVector(allResults.m_hitNormalWorld[i]);
        auto collisionObject = allResults.m_collisionObjects[i];
        auto entityHolder = static_cast<EntityHolder*>(collisionObject->getUserPointer());
        auto entity = entityHolder->entity;
        if (entity.valid()) {
          auto gui = core::Core::Instance->get<Gui>();
          gui->entityBrowser.selectEntity(entity);
          return;
        }
 
      }
    } 

    std::tuple<ecs::Entity, render::Camera*> cameraAndEntity(ecs::EntityManager* manager) {
      for (auto entity : manager->with({PerspectiveCameraComponent})) {
        return {entity, entity.get<render::PerspectiveCamera>(PerspectiveCameraComponent)};
      }
      return {ecs::Entity::Invalid, nullptr};
    }

    btDefaultCollisionConfiguration collisionConfiguration;

    btCollisionDispatcher dispatcher;

    btDbvtBroadphase overlappingPairCache;

    btSequentialImpulseConstraintSolver solver;

    btDiscreteDynamicsWorld dynamicsWorld;

    std::vector<EntityHolder> entities;
  };


  class SceneController : public core::Service, public messaging::MessageListener<input::KeyEvent> {
  public:
    SceneController() : Service("SceneController") {}

    void start() {
      core::Core::Instance->get<messaging::Messaging>()->listen<input::KeyEvent>(this);
    }

    inline bool on(const input::KeyEvent& event) {
      // if (event.key() == input::Key::l && event.type() == input::KeyEventType::press) {
      //   std::cout << "Loading scene...\n";
      //   auto serializer = core::Core::Instance->get<serialize::SceneSerializer>();
      //   serializer->load({{"scenes/test.wscene.json"}}, scene::Scene::current());
      // }
      // if (event.key() == input::Key::k && event.type() == input::KeyEventType::press) {
      //   std::cout << "Writing scene...\n";
      //   auto serializer = core::Core::Instance->get<serialize::SceneSerializer>();
      //   serializer->store({{"scenes/test2.wscene.json"}}, scene::Scene::current());
      // }
      // if (event.key() == input::Key::n && event.type() == input::KeyEventType::press && event.set(input::KeyModifier::super)) {
      //   std::cout << "New scene...\n";
      //   memory::Allocator::Permanent()->clear();
      //   core::Core::Instance->get<scene::Manager>()->reset();
      // }
      // if (event.key() == input::Key::s && event.type() == input::KeyEventType::press && event.set(input::KeyModifier::super)) {
      //   std::cout << "Save scene...\n";
      //   auto serializer = core::Core::Instance->get<serialize::SceneSerializer>();
      //   serializer->store({{"scenes/test2.wscene.json"}}, scene::Scene::current());
      // }
      // if (event.key() == input::Key::o && event.type() == input::KeyEventType::press && event.set(input::KeyModifier::super)) {
      //   std::cout << "Open scene...\n";
      //   memory::Allocator::Permanent()->clear();
      //   core::Core::Instance->get<scene::Manager>()->reset();
      // }
    }
  };
}
