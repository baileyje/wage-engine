#pragma once

#include "engine.h"

using namespace wage;

class FpsDisplay : public ecs::System {

public:
  FpsDisplay(ui::UiLabel* label) : System(), label(label) {}

  void update(const ecs::SystemContext& context) {
    auto frameTime = core::Core::Instance->get<render::Renderer>()->averageFrameTime();
    std::ostringstream os;
    // os.precision(2);
    os << "Frame Time: " << frameTime << "ms";
    label->set(os.str());
  }

private:
  ui::UiLabel* label;
};

class PosDisplay : public ecs::System {

public:
  PosDisplay(ui::UiLabel* label) : System(), label(label) {}

  void update(const ecs::SystemContext& context) {
    auto player = *(scene::Scene::current().entities().with({PlayerComponent}).begin());
    auto pos = player.get<math::Transform>(TransformComponent)->localPosition;
    std::ostringstream os;
    os << "Player POS: " << int(pos.x) << ":" << int(pos.y) << ":" << int(pos.z);
    label->set(os.str());
  }

private:
  ui::UiLabel* label;
};

class CamPosDisplay : public ecs::System {

public:
  CamPosDisplay(ui::UiLabel* label) : System(), label(label) {}

  void update(const ecs::SystemContext& context) {
    auto camera = *(scene::Scene::current().entities().with({PerspectiveCameraComponent}).begin());
    auto pos = camera.get<math::Transform>(TransformComponent)->localPosition;
    std::ostringstream os;
    os << "Cam POS: " << int(pos.x) << ":" << int(pos.y) << ":" << int(pos.z);
    label->set(os.str());
  }

private:
  ui::UiLabel* label;
};

void setupHud(scene::Scene& scene) {
  render::FontSpec font("ARCADE.TTF", 20);
  auto fpsLabelEntity = scene.entities().create();
  auto fpsLabel = fpsLabelEntity.assign<ui::UiLabel>(UiLabelComponent, ui::UiFrame(Vector2(20, 0), Vector2(300, 100)), "FPS: ", font, component::Color::White);
  scene.systems().create<FpsDisplay>(fpsLabel);

  auto posLabelEntity = scene.entities().create();
  auto posLabel = posLabelEntity.assign<ui::UiLabel>(UiLabelComponent, ui::UiFrame(Vector2(200, 0), Vector2(300, 100)), "Player POS: ", font, component::Color::White);
  scene.systems().create<PosDisplay>(posLabel);

  auto camPosLabelEntity = scene.entities().create();
  auto camPosLabel = camPosLabelEntity.assign<ui::UiLabel>(UiLabelComponent, ui::UiFrame(Vector2(600, 0), Vector2(300, 100)), "Cam POS: ", font, component::Color::White);
  scene.systems().create<CamPosDisplay>(camPosLabel);

  auto targetLabelEntity = scene.entities().create();
  // auto platform = core::Core::Instance->get<platform::Platform>();
  // auto window = platform->window();
  targetLabelEntity.assign<ui::UiLabel>(UiLabelComponent, ui::UiFrame(Vector2(1024 / 2.0, 768 * 0.9 / 2.0), Vector2(20, 20)), "-X-", font, component::Color::White);

  // auto buttonEntity = entityManager->create();
  // buttonEntity.assign<UiButton>(UiFrame(Vector2(600, 0), Vector2(300, 100)), component::Color::Clear,
  //   Texture("textures/button.png"),
  //   Texture("textures/default.png")
  // );
}
