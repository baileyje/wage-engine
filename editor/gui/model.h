#pragma once

#include "imgui.h"
#include "engine.h"
#include "transform.h"

namespace wage::editor {

  class ModelWidget {
  public:
    ModelWidget() : ModelWidget(nullptr, nullptr) {
    }

    ModelWidget(math::Transform* transform, render::MeshSpec* meshSpec) : transform(transform), meshSpec(meshSpec), meshTransform(nullptr) {
      if (meshSpec) {
        meshSpec->key().copy(meshNameInput, 64);
        meshTransform = TransformWidget(&meshSpec->transform);
      } else {
        meshNameInput[0] = '\0';
      }
    }

    void apply() {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Transform");
      transform.apply();

      // if (!meshSpec) return;
      // ImGui::TextColored(ImVec4(1, 1, 0, 1), "Mesh");
      // if (ImGui::InputText("Mesh", meshNameInput, 64)) {
      //   if (meshSpec) meshSpec->key(meshNameInput);
      // }
      meshTransform.apply();
    }

  private:
    TransformWidget transform;
    TransformWidget meshTransform;
    render::MeshSpec* meshSpec;

    char meshNameInput[64] = "";
  };
}