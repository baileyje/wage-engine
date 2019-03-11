
#include "ecs/system.h"
#include "ecs/entity_manager.h"
#include "render/renderer.h"
#include "ui.h"
#include "input/mouse_button_event.h"
#include "platform/platform.h"

namespace wage {

  class UiSystem : public System, MessageListener<MouseButtonEvent> {

  public:
    UiSystem() : System("UI") {}

    void start(SystemContext* context) {
      Core::Instance->get<Messaging>()->listen<MouseButtonEvent>(this);
    }

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
      auto buttonTexture = button->state() == UiButton::State::pressed ? button->pressedTexture() : button->texture();
      Core::Instance->get<Renderer>()->renderSprite(
          button->frame().position(), button->frame().size(), button->color(), buttonTexture);
    }

    bool on(const MouseButtonEvent& event) {
      EntityManager* manager = Core::Instance->get<EntityManager>();
      auto window = Core::Instance->get<Platform>()->window();
      for (auto entity : manager->registry()->with<UiButton>()) {
        auto button = entity.get<UiButton>();
        auto frame = button->frame();
        auto pos = Vector2(
          event.position().x * window->scale(),
          window->height() - event.position().y * window->scale()
        );
        if (
            pos.x >= frame.position().x && pos.x <= frame.position().x + frame.size().x &&
            pos.y >= frame.position().y && pos.y <= frame.position().y + frame.size().y
        ) {
          if (event.type() == MouseButtonEvent::Type::press) {
            button->state(UiButton::State::pressed);
          } else {
            button->state(UiButton::State::released);
            // TODO: Dispatch button press event
          }
          return true;
        }
      }
      return false;
    }
  };
}
