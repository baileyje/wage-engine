
#include "ecs/system.h"
#include "ecs/entity_manager.h"
#include "render/renderer.h"
#include "ui.h"

namespace wage {

  class UiSystem : public System {

  public:
    UiSystem() : System("UI") {}

    void update(SystemContext* context) {
      EntityManager* manager = Core::Instance->get<EntityManager>();
      for (auto entity : manager->registry()->with<UiLabel>()) {
        render(entity.get<UiLabel>());
      }
      for (auto entity : manager->registry()->with<UiButton>()) {
        render(entity.get<UiButton>());
      }
    }

    void render(Reference<UiLabel> label) {
      Core::Instance->get<Renderer>()->renderText(
          label->frame().position(), label->text(), label->font(), label->color());
    }

    void render(Reference<UiButton> button) {
      Core::Instance->get<Renderer>()->renderSprite(
          button->frame().position(), button->frame().size(), button->color(), button->texture());
    }
  };
}
