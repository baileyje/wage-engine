#pragma once

#include <filesystem>
#include <functional>

#include "imgui.h"

namespace wage::editor {

  class AssetBrowser {
  public:
    AssetBrowser(std::string name, std::filesystem::path resourcePath) : name(name), resourcePath(resourcePath) {
      resfresh();
    }

    void resfresh() {
      keys.clear();
      for (const auto& entry : std::filesystem::directory_iterator(resourcePath))
        keys.push_back(entry.path().filename());
    }

    void apply() {
      if (!open) return;
      ImGui::Begin((name + " Browser").c_str(), &open);
      for (auto key : keys) {
        ImGui::Text("%s", key.c_str());
        if (ImGui::IsItemClicked()) {
          onSelect(key);
        }
      }
      ImGui::End();
    }

    std::string name;

    std::filesystem::path resourcePath;

    std::vector<std::string> keys;

    bool open = false;

    std::function<void(std::string key)> onSelect;
  };
}