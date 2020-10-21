
#pragma once

#include "imgui.h"
#include "engine.h"
#include "rigid_body.h"
#include "collider.h"

namespace wage::editor {

  class PhysicsWidget {
  public:
    PhysicsWidget() : PhysicsWidget(nullptr, nullptr) {
    }

    PhysicsWidget(physics::RigidBody* rigidBody, physics::Collider* collider) : rigidBody(rigidBody), collider(collider) {}

    void apply() {
      ImGui::BeginGroup();
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Rigid Body");
      rigidBody.apply();
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Collider");
      collider.apply();
      ImGui::EndGroup();
    }

  private:    
    RigidBodyWidget rigidBody;
    ColliderWidget collider;
  };
}