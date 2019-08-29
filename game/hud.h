#ifndef HUD_H
#define HUD_H

#include "engine.h"

using namespace wage;


class FpsDisplay : public System {

public:
  FpsDisplay(UiLabel* label) : System(), label(label), lastTime(0), frames(0) {}

  void update(const SystemContext& context) {
    double currentTime = context.time();
    frames++;
    if (currentTime - lastTime >= 1.0) {
      std::ostringstream os;
      os << "FPS: " << frames;
      std::cout << "FPS: " << frames << std::endl;
      label->set(os.str());
      frames = 0;
      lastTime = currentTime;
    }
  }

private:
  UiLabel* label;

  double lastTime;

  int frames;
};

class PosDisplay : public System {

public:
  PosDisplay(UiLabel* label) : System(), label(label) {}

  void update(const SystemContext& context) {
    auto player = *Core::Instance->get<EntityManager>()->with<Player>().begin();
    auto pos = player.get<Transform>()->position();
    std::ostringstream os;
    os << "POS: " << int(pos.x) << ":" << int(pos.y) << ":" << int(pos.z);
    label->set(os.str());
  }

private:
  UiLabel* label;
};

void setupHud(EntityManager* entityManager, SystemManager* systemManager) {
  Font font("fonts/ARCADE.TTF", 30);
  auto fpsLabelEntity = entityManager->create();
  auto fpsLabel = fpsLabelEntity.assign<UiLabel>(UiFrame(Vector2(20, 0), Vector2(300, 100)), "FPS: ", font, Color::White);
  systemManager->create<FpsDisplay>(fpsLabel.get());

  auto posLabelEntity = entityManager->create();
  auto posLabel = posLabelEntity.assign<UiLabel>(UiFrame(Vector2(200, 0), Vector2(300, 100)), "POS: ", font, Color::White);
  systemManager->create<PosDisplay>(posLabel.get());

  // auto buttonEntity = entityManager->create();
  // buttonEntity.assign<UiButton>(UiFrame(Vector2(600, 0), Vector2(300, 100)), Color::Clear, 
  //   Texture("textures/button.png"),
  //   Texture("textures/default.png")
  // );
}

#endif // HUD_H