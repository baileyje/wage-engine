#ifndef HUD_H
#define HUD_H

#include "engine.h"

using namespace wage;


class FpsDisplay : public System {

public:
  FpsDisplay(Reference<UiLabel> label) : System("FPS"), label(label), lastTime(0), frames(0) {}

  void update(SystemContext* context) {
    double currentTime = context->time();
    frames++;
    if (currentTime - lastTime >= 1.0) {
      std::cout << "FPS: " << frames << std::endl;
      std::ostringstream os;
      os << "FPS: " << frames;
      label->set(os.str());
      frames = 0;
      lastTime = currentTime;
    }
  }

private:
  Reference<UiLabel> label;

  double lastTime;

  int frames;
};

class PosDisplay : public System {

public:
  PosDisplay(Reference<UiLabel> label) : System("POS"), label(label) {}

  void update(SystemContext* context) {
    auto player = *Core::Instance->get<EntityManager>()->registry()->with<Player>().begin();
    auto pos = player.get<Transform>()->position();
    std::ostringstream os;
    os << "POS: " << int(pos.x) << ":" << int(pos.y) << ":" << int(pos.z);
    label->set(os.str());
  }

private:
  Reference<UiLabel> label;
};

void setupHud(EntityManager* entityManager, SystemManager* systemManager) {
  Font font("fonts/ARCADE.TTF", 60);
  auto fpsLabelEntity = entityManager->create();
  auto fpsLabel = fpsLabelEntity.assign<UiLabel>(UiFrame(Vector2(20, 0), Vector2(300, 100)), "FPS: ", font, Color::White);
  systemManager->create<FpsDisplay>(fpsLabel);

  auto posLabelEntity = entityManager->create();
  auto posLabel = posLabelEntity.assign<UiLabel>(UiFrame(Vector2(300, 0), Vector2(300, 100)), "POS: ", font, Color::White);
  systemManager->create<PosDisplay>(posLabel);

  auto buttonEntity = entityManager->create();
  buttonEntity.assign<UiButton>(UiFrame(Vector2(600, 0), Vector2(300, 100)), Color::Clear, Texture("textures/button.png"));
}

#endif // HUD_H