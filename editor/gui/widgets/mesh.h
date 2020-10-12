#pragma once

#include "imgui.h"
#include "engine.h"
#include "transform.h"
#include "../windows/asset_browser.h"

namespace wage::editor {

  class MeshWidget {
  public:
    MeshWidget() : MeshWidget(nullptr, nullptr) {
    }

    MeshWidget(render::MeshSpec* meshSpec, AssetBrowser* meshBrowser) : transform(nullptr), meshSpec(meshSpec), meshBrowser(meshBrowser) {
      if (meshSpec) {
        transform = TransformWidget(&meshSpec->transform);
      }
    }

    void apply() {
      if (!meshSpec) return;
      ImGui::PushID("MESH");
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Mesh");

      ImGui::LabelText("Mesh Key", "%s", meshSpec->key().c_str());
      if (ImGui::IsItemClicked()) {
        if (meshBrowser) {
          meshBrowser->onSelect = [&](std::string key) {
            std::cout << "Selected one: " << key << std::endl;
            meshSpec->key(key);
          };
          meshBrowser->open = true;
        }
      }
      transform.apply();
      ImGui::PopID();
    }

  private:
    TransformWidget transform;

    render::MeshSpec* meshSpec;

    AssetBrowser* meshBrowser;
  };
}