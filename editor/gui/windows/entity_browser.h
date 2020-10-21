#pragma once

#include "imgui.h"

#include "engine.h"

#include "../widgets/model.h"
#include "../widgets/physics.h"
#include "asset_browser.h"

namespace wage::editor {

  class EntityBrowser {
  public:
    EntityBrowser(AssetBrowser* meshBrowser, AssetBrowser* textureBrowser) : meshBrowser(meshBrowser), textureBrowser(textureBrowser) {}

    void
    apply() {
      if (ImGui::Begin("Entities", &open, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
          if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Close")) open = false;
            ImGui::EndMenu();
          }
          ImGui::EndMenuBar();
        }

        // Left
        {
          ImGui::BeginGroup();
          ImGui::BeginChild("left pane", ImVec2(120, -ImGui::GetFrameHeightWithSpacing()), true);
          auto& entities = scene::Scene::current().entities();
          for (auto entity : entities.with({TransformComponent})) {

            ImGui::Text("Entity: %d", entity.id().id());
            if (ImGui::IsItemClicked()) {
              selectEntity(entity);
              camToEntity(entity);
            }
          }
          ImGui::EndChild();
          if (ImGui::Button("Add Entity")) {
            addEntity();
          }
          ImGui::EndGroup();
        }
        ImGui::SameLine();

        // Right
        {
          ImGui::BeginGroup();
          if (selectedEntity.valid()) {
            ImGui::BeginChild("entity view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text("Entity: %d", selectedEntity.id().id());
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
              if (ImGui::BeginTabItem("Model")) {
                modelWidget.apply();
                ImGui::EndTabItem();
              }
              if (ImGui::BeginTabItem("Physics")) {
                physicsWidget.apply();
                ImGui::EndTabItem();
              }

              // if (ImGui::BeginTabItem("Extra")) {
              // }
              ImGui::EndTabBar();
            }
            ImGui::EndChild();
          }
          ImGui::EndGroup();
        }
      }
      ImGui::End();
    }

    bool open = true;

    void selectEntity(ecs::Entity entity) {
      selectedEntity = entity;
      auto transform = entity.get<math::Transform>(TransformComponent);
      render::MeshSpec* meshSpec = nullptr;
      if (entity.has(MeshComponent)) {
        meshSpec = entity.get<render::MeshSpec>(MeshComponent);
      }
      render::MaterialSpec* materialSpec = nullptr;
      if (entity.has(MaterialComponent)) {
        materialSpec = entity.get<render::MaterialSpec>(MaterialComponent);
      }
      modelWidget = ModelWidget(transform, meshSpec, materialSpec, meshBrowser, textureBrowser);
      open = true;

      physics::RigidBody* rigidBody = nullptr;
      if (entity.has(RigidBodyComponent))
        rigidBody = entity.get<physics::RigidBody>(RigidBodyComponent);
      physics::Collider* collider = nullptr;
      if (entity.has(ColliderComponent))
        collider = entity.get<physics::Collider>(ColliderComponent);
      
      physicsWidget = PhysicsWidget(rigidBody, collider);
    }

    void camToEntity(ecs::Entity entity) {
      auto transform = entity.get<math::Transform>(TransformComponent);
      auto& entities = scene::Scene::current().entities();
      auto cameraEntity = *entities.with({PerspectiveCameraComponent}).begin();
      auto camTransform = cameraEntity.get<math::Transform>(TransformComponent);
      camTransform->localPosition = transform->localPosition + math::Vector3(0, 0, -10);
      camTransform->localRotation = math::Quaternion::fromEulers(math::Vector3::Forward);
    }

  private:
    void addEntity() {
      auto& entities = scene::Scene::current().entities();
      auto entity = entities.create();
      entity.assign<math::Transform>(TransformComponent);
      entity.assign<render::MeshSpec>(MeshComponent, "cube.obj");
      entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("default.png"));
      auto collider = entity.assign<physics::Collider>(ColliderComponent, physics::ColliderType::box);
      collider->transform.localScale = {2, 2, 2};
      entity.assign<physics::RigidBody>(RigidBodyComponent, 2.0);
      camToEntity(entity);
      selectEntity(entity);
    }
    ecs::Entity selectedEntity = ecs::Entity::Invalid;

    AssetBrowser* meshBrowser;

    AssetBrowser* textureBrowser;

    ModelWidget modelWidget;

    PhysicsWidget physicsWidget;
  };
}