#pragma once

#include "imgui.h"
#include "engine.h"
#include "transform.h"
#include "../windows/asset_browser.h"

namespace wage::editor {

  class MaterialWidget {
  public:
    MaterialWidget() : MaterialWidget(nullptr, nullptr) {
    }

    MaterialWidget(render::MaterialSpec* materialSpec, AssetBrowser* textureBrowser) : materialSpec(materialSpec), textureBrowser(textureBrowser) {      
    }

    void apply() {
      if (!materialSpec) return;
      ImGui::PushID("MATERIAL");
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Material");
      ImGui::LabelText("Texture Key", "%s", materialSpec->texture().key().c_str());
      if (ImGui::IsItemClicked()) {
        if (textureBrowser) {
          textureBrowser->onSelect = [&](std::string key) {
            materialSpec->texture({key});
          };
          textureBrowser->open = true;
        }
      }
      ImGui::PopID();
    }

  private:
    render::MaterialSpec* materialSpec;
    AssetBrowser* textureBrowser;
   
  };
}