#pragma once

#include "imgui.h"

#include "engine.h"

#include "../widgets/model.h"
#include "asset_browser.h"

namespace wage::editor {

  class EntityBrowser {
  public:
    EntityBrowser(AssetBrowser* meshBrowser, AssetBrowser* textureBrowser) : meshBrowser(meshBrowser), textureBrowser(textureBrowser) {}

    void
    apply() {
      ImGui::SetNextWindowSize(ImVec2(380, 600));
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

  private:
    void addEntity() {
      auto& entities = scene::Scene::current().entities();
      auto entity = entities.create();
      entity.assign<math::Transform>(TransformComponent);
      entity.assign<render::MeshSpec>(MeshComponent, "cube.obj");
      entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("default.png"));
      // auto collider = entity.assign<physics::Collider>(ColliderComponent, physics::ColliderType::box);
      // collider->transform.localScale = {2, 2, 2};
    }

    ecs::Entity selectedEntity = ecs::Entity::Invalid;

    AssetBrowser* meshBrowser;

    AssetBrowser* textureBrowser;

    ModelWidget modelWidget;
  };
}