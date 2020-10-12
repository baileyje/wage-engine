#pragma once

#include "imgui.h"
#include "engine.h"
#include "transform.h"
#include "mesh.h"
#include "material.h"
#include "../windows/asset_browser.h"

namespace wage::editor {

  class ModelWidget {
  public:
    ModelWidget() : ModelWidget(nullptr, nullptr, nullptr, nullptr, nullptr) {
    }

    ModelWidget(math::Transform* transform, render::MeshSpec* meshSpec, render::MaterialSpec* materialSpec, AssetBrowser* meshBrowser, AssetBrowser* textureBrowser) : transform(transform), mesh(meshSpec, meshBrowser), material(materialSpec, textureBrowser) {
    }

    void apply() {

      ImGui::BeginGroup();
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Transform");
      transform.apply();
      ImGui::EndGroup();

      mesh.apply();
      material.apply();
    }

  private:
    TransformWidget transform;
    MeshWidget mesh;
    MaterialWidget material;
  };
}