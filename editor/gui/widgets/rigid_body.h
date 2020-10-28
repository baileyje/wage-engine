#pragma once

#include "imgui.h"

#include "engine.h"

namespace wage::editor {

  class RigidBodyWidget {
  public:
    RigidBodyWidget(physics::RigidBody* body = nullptr) : body(body) {
      if (body) {
        switch (body->type) {
        case physics::RigidBody::Type::dynamic:
          selectedType = 0;
          break;
        case physics::RigidBody::Type::kinematic:
          selectedType = 1;
          break;
        case physics::RigidBody::Type::immovable:
          selectedType = 2;
          break;
        }
      }
    }

    void apply() {
      if (!body) return;
      ImGui::PushID("RIGIDBODY");
      ImGui::InputFloat("Mass", &body->mass);
      const char* items[] = {"Dynamic", "Kinematic", "Static"};
      if (ImGui::Combo("Type", &selectedType, items, IM_ARRAYSIZE(items))) {
        switch (selectedType) {
        case 0:
          body->type = physics::RigidBody::Type::dynamic;
          break;
        case 1:
          body->type = physics::RigidBody::Type::kinematic;
          break;
        case 2:
          body->type = physics::RigidBody::Type::immovable;
          break;
        }
      }
      ImGui::Checkbox("Affected By Gravity", &body->affectedByGravity);
      ImGui::PopID();
    }

  private:
    physics::RigidBody* body = nullptr;
    int selectedType = 0;
  };
}