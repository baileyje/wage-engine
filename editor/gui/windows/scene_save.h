#pragma once

#include <filesystem>
#include <functional>

#include "imgui.h"

#include "engine.h"

namespace wage::editor {

  class SceneSave {
  public:
    SceneSave(std::filesystem::path scenePath) : scenePath(scenePath) {
    }

    void apply() {
      if (open) ImGui::OpenPopup("Save Scene");
      ImVec2 center = ImGui::GetMainViewport()->GetCenter();
      ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
      ImGui::SetNextWindowSizeConstraints(ImVec2(300, 150), ImVec2(FLT_MAX, FLT_MAX));
      if (ImGui::BeginPopupModal("Save Scene", &open, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::BeginGroup();
        ImGui::BeginChild("form", ImVec2(300, 120), true);
        static char name[255] = {};
        if (ImGui::InputText("Scene Name", name, IM_ARRAYSIZE(name))) {
        }
        ImGui::EndChild();
        if (ImGui::Button("Save")) {
          save(name);
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          hide();
        }
        ImGui::EndGroup();
        ImGui::EndPopup();
      }
    }

    void show() {
      open = true;
    }

    void hide() {
      ImGui::CloseCurrentPopup();
      open = false;
    }

    void save(char* name) {
      std::cout << "Save... " << name << "\n";
      auto serializer = core::Core::Instance->get<serialize::SceneSerializer>();
      serializer->store({{"scenes", name}}, scene::Scene::current());
      hide();
    }

    std::filesystem::path scenePath;

    std::string name;

    bool open = false;
  };
}