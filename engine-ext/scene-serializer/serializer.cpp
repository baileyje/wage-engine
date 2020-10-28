#include "scene-serializer/serializer.h"

#include <json/json.h>

namespace wage::serialize {

  SceneSerializer::SceneSerializer() : Service("SceneSerializer") {}

  void loadTransform(Json::Value& transformNode, math::Transform* transform) {
    if (!transformNode["position"].isNull()) {
      transform->localPosition.x = transformNode["position"]["x"].asFloat();
      transform->localPosition.y = transformNode["position"]["y"].asFloat();
      transform->localPosition.z = transformNode["position"]["z"].asFloat();
    }
    if (!transformNode["scale"].isNull()) {
      transform->localScale.x = transformNode["scale"]["x"].asFloat();
      transform->localScale.y = transformNode["scale"]["y"].asFloat();
      transform->localScale.z = transformNode["scale"]["z"].asFloat();
    }
    if (!transformNode["rotation"].isNull()) {
      transform->localRotation.x = transformNode["rotation"]["x"].asFloat();
      transform->localRotation.y = transformNode["rotation"]["y"].asFloat();
      transform->localRotation.z = transformNode["rotation"]["z"].asFloat();
      transform->localRotation.w = transformNode["rotation"]["w"].asFloat();
    }
  }

  void SceneSerializer::start() {
    fileSystem = core::Core::Instance->get<fs::FileSystem>();
  }

  void SceneSerializer::load(fs::File::Path path, scene::Scene& scene) {
    if (fileSystem->exists(path)) {
      auto data = fileSystem->read(path, memory::Allocator::Temporary());
      Json::Value root;
      Json::Reader reader;
      bool parsingSuccessful = reader.parse((const char*)data.data(), (const char*)data.data() + data.length(), root);
      if (!parsingSuccessful) {
        core::Logger::error("Error parsing the string");
        return;
      }
      auto entities = root["entities"];
      for (Json::Value::const_iterator entityNode = entities.begin(); entityNode != entities.end(); entityNode++) {
        std::cout << entityNode.key() << ": " << *entityNode << std::endl;
        // TODO:  Force ID here
        auto entity = scene.entities().create();
        auto transformNode = (*entityNode)["transform"];
        if (!transformNode.isNull()) {
          auto transform = entity.assign<math::Transform>(TransformComponent);
          loadTransform(transformNode, transform);
        }
        auto modelNode = (*entityNode)["model"];
        if (!modelNode.isNull()) {
          auto meshNode = modelNode["mesh"];
          if (!meshNode.isNull()) {
            std::cout << ": " << meshNode["key"].asString() << std::endl;
            auto mesh = entity.assign<render::MeshSpec>(MeshComponent, meshNode["key"].asString());
            if (!meshNode["transform"].isNull()) {
              loadTransform(meshNode["transform"], &mesh->transform);
            }
          }
          auto materialNode = modelNode["material"];
          if (!materialNode.isNull()) {
            if (!materialNode["texture"].isNull()) {
              entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec(materialNode["texture"]["key"].asString()));
            }
          }
        }
        auto physicsNode = (*entityNode)["physics"];
        if (!physicsNode.isNull()) {
          if (!physicsNode["rigidBody"].isNull()) {
            auto typeString = physicsNode["rigidBody"]["type"].asString();
            physics::RigidBody::Type type = physics::RigidBody::Type::dynamic;
            if (typeString == "static") type = physics::RigidBody::Type::immovable;
            if (typeString == "kinematic") type = physics::RigidBody::Type::kinematic;
            entity.assign<physics::RigidBody>(RigidBodyComponent, physicsNode["rigidBody"]["mass"].asFloat(), type);
          }
          if (!physicsNode["collider"].isNull()) {
            auto typeString = physicsNode["collider"]["type"].asString();
            physics::Collider::Type type = physics::Collider::Type::box;
            if (typeString == "sphere") type = physics::Collider::Type::sphere;
            if (typeString == "mesh") type = physics::Collider::Type::mesh;
            auto collider = entity.assign<physics::Collider>(ColliderComponent, type);
            if (!physicsNode["collider"]["transform"].isNull()) {
              loadTransform(physicsNode["collider"]["transform"], &collider->transform);
            }
          }
        }
      }
    }
  }

  void storeTransform(Json::Value& transformNode, math::Transform* transform) {
    transformNode["position"]["x"] = transform->localPosition.x;
    transformNode["position"]["y"] = transform->localPosition.y;
    transformNode["position"]["z"] = transform->localPosition.z;
    transformNode["scale"]["x"] = transform->localScale.x;
    transformNode["scale"]["y"] = transform->localScale.y;
    transformNode["scale"]["z"] = transform->localScale.z;
    transformNode["rotation"]["x"] = transform->localRotation.x;
    transformNode["rotation"]["y"] = transform->localRotation.y;
    transformNode["rotation"]["z"] = transform->localRotation.z;
    transformNode["rotation"]["w"] = transform->localRotation.w;
  }

  void SceneSerializer::store(fs::File::Path path, scene::Scene& scene) {
    Json::Value root;
    auto& entities = scene.entities();
    Json::Value entitiesNode;
    // TODO: Get all entityies....
    for (auto entity : entities.with({TransformComponent})) {
      if (entity.has(NoSerializeComponent)) continue;
        Json::Value entityNode;
      if (entity.has(TransformComponent)) {
        auto transform = entity.get<math::Transform>(TransformComponent);
        Json::Value transformNode;
        storeTransform(transformNode, transform);
        entityNode["transform"] = transformNode;
      }
      if (entity.has(MeshComponent) || entity.has(MaterialComponent)) {
        Json::Value modelNode;
        if (entity.has(MeshComponent)) {
          Json::Value meshNode;
          auto mesh = entity.get<render::MeshSpec>(MeshComponent);
          meshNode["key"] = mesh->key();
          Json::Value transformNode;
          storeTransform(transformNode, &mesh->transform);
          meshNode["transform"] = transformNode;
          modelNode["mesh"] = meshNode;
        }
        if (entity.has(MaterialComponent)) {
          Json::Value materialNode;
          auto material = entity.get<render::MaterialSpec>(MaterialComponent);
          materialNode["texture"]["key"] = material->texture().key();
          modelNode["material"] = materialNode;
        }
        entityNode["model"] = modelNode;
      }
      if (entity.has(RigidBodyComponent) || entity.has(ColliderComponent)) {
        Json::Value physicsNode;
        if (entity.has(RigidBodyComponent)) {
          Json::Value rigidBodyNode;
          auto rigidBody = entity.get<physics::RigidBody>(RigidBodyComponent);
          rigidBodyNode["mass"] = rigidBody->mass;
          auto typeStr = "dynamic";
          switch(rigidBody->type) {
            case physics::RigidBody::Type::immovable:
              typeStr = "static";
              break;
            case physics::RigidBody::Type::kinematic:
              typeStr = "kinematic";
              break;
          }
          rigidBodyNode["type"] = typeStr;
          physicsNode["rigidBody"] = rigidBodyNode;
        }
        if (entity.has(ColliderComponent)) {
          Json::Value colliderNode;
          auto collider = entity.get<physics::Collider>(ColliderComponent);
          auto typeStr = "box";
          switch (collider->type) {
          case physics::Collider::Type::sphere:
            typeStr = "sphere";
            break;
          case physics::Collider::Type::mesh:
            typeStr = "mesh";
            break;
          }
          colliderNode["type"] = typeStr;
          Json::Value transformNode;
          storeTransform(transformNode, &collider->transform);
          colliderNode["transform"] = transformNode;
          physicsNode["collider"] = colliderNode;
        }
        entityNode["physics"] = physicsNode;
      }
      entitiesNode.append(entityNode);
    }
    
    root["entities"] = entitiesNode;
    Json::StyledWriter writer;
    auto json = writer.write(root);
    std::cout << "JSON: " << json << std::endl;
    std::cout << "Path: " << path.string() << std::endl;

    fileSystem->write(path, (unsigned char*)json.c_str(), sizeof(char) * json.size());
  }
}