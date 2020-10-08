#pragma once

#include "imgui.h"

#include "engine.h"

namespace wage::editor {

  class TransformWidget {
  public:
    TransformWidget(math::Transform* transform) : transform(transform) {
      if (transform) {
        auto transformEulers = transform->localRotation.eulerAngles();
        eulers[0] = transformEulers.x;
        eulers[1] = transformEulers.y;
        eulers[2] = transformEulers.z;
      }
    }

    void apply() {
      if (!transform) return;
      ImGui::InputFloat3("Position", &transform->localPosition.x);
      ImGui::InputFloat3("Scale", &transform->localScale.x);
      if (ImGui::InputFloat3("Rotation", eulers)) {
        transform->localRotation = math::Quaternion::fromEulers({eulers[0], eulers[1], eulers[2]});
      }
    }    
  private:
    math::Transform* transform = nullptr;
    float eulers[3] = {0, 0, 0};
  };
}