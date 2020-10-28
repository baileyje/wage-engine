#pragma once

#include "imgui.h"

#include "engine.h"
#include "transform.h"

namespace wage::editor {

  class ColliderWidget {
  public:
    ColliderWidget(physics::Collider* collider = nullptr) : collider(collider), transform(nullptr) {
      if (collider) {
        switch (collider->type) {
        case physics::Collider::Type::box:
          selectedType = 0;
          break;
        case physics::Collider::Type::sphere:
          selectedType = 1;
          break;
        case physics::Collider::Type::mesh:
          selectedType = 2;
          break;
        }
        transform = TransformWidget(&collider->transform);
      }
    }

    void apply() {
      if (!collider) return;
      ImGui::PushID("COLLIDER");
      const char* items[] = {"Box", "Sphere", "Mesh"};
      if (ImGui::Combo("Type", &selectedType, items, IM_ARRAYSIZE(items))) {
        switch (selectedType) {
        case 0:
          collider->type = physics::Collider::Type::box;
          break;
        case 1:
          collider->type = physics::Collider::Type::sphere;
          break;
        case 2:
          collider->type = physics::Collider::Type::mesh;
          break;
        }
      }
      transform.apply();
      ImGui::PopID();
    }

  private:
    physics::Collider* collider = nullptr;
    TransformWidget transform;
    int selectedType = 0;
  };
}