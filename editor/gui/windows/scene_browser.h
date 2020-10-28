#pragma once

#include <filesystem>
#include <functional>

#include "imgui.h"

namespace wage::editor {

  class SceneBrowser {
  public:
    SceneBrowser(std::filesystem::path scenePath) : scenePath(scenePath) {
      refresh();
    }

    void refresh() {
      keys.clear();
      for (const auto& entry : std::filesystem::directory_iterator(scenePath))
        keys.push_back(entry.path().filename());
    }

    void apply() {
      if (open) ImGui::OpenPopup("Select Scene");
      ImVec2 center = ImGui::GetMainViewport()->GetCenter();
      ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
      ImGui::SetNextWindowSizeConstraints(ImVec2(300, 150), ImVec2(FLT_MAX, FLT_MAX));
      if (ImGui::BeginPopupModal("Select Scene", &open, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::BeginGroup();
        ImGui::BeginChild("scenes", ImVec2(300, 120), true);
         for (auto key : keys) {
          ImGui::Text("%s", key.c_str());
          if (ImGui::IsItemClicked()) {
            onSelect(key);
            hide();
          }
        }
        ImGui::EndChild();
        if (ImGui::Button("Cancel")) {          
          hide();
        }
        ImGui::EndGroup();
        ImGui::EndPopup();
      }
    }

    void show() {
      refresh();
      open = true;
    }

    void hide() {
      ImGui::CloseCurrentPopup();
      open = false;
    }

    std::filesystem::path scenePath;

    std::vector<std::string> keys;

    bool open = false;

    std::function<void(std::string key)> onSelect;
  };
}