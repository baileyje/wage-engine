#pragma once

#include "ecs/system.h"
#include "ecs/manager.h"
#include "render/renderer.h"
#include "ui.h"
#include "input/mouse_button_event.h"
#include "platform/platform.h"

namespace wage {
  namespace ui {

    class UiSystem : public ecs::System, messaging::MessageListener<input::MouseButtonEvent> {

    public:
      UiSystem() : System() {}

      void start(const ecs::SystemContext& context) {
        core::Core::Instance->get<messaging::Messaging>()->listen<input::MouseButtonEvent>(this);
      }

      void update(const ecs::SystemContext& context) {
        auto manager = core::Core::Instance->get<ecs::EntityManager>();
        for (auto entity : manager->with({UiLabelComponent})) {
          render(entity.get<UiLabel>(UiLabelComponent));
        }
        for (auto entity : manager->with({UiButtonComponent})) {
          render(entity.get<UiButton>(UiButtonComponent));
        }
      }

      void render(UiLabel* label) {
        core::Core::Instance->get<render::Renderer>()->renderText(
            label->frame().position(), label->text(), label->font(), label->color());
      }

      void render(UiButton* button) {
        auto buttonTexture = button->state() == UiButton::State::pressed ? button->pressedTexture() : button->texture();
        core::Core::Instance->get<render::Renderer>()->renderSprite(
            button->frame().position(), button->frame().size(), button->color(), buttonTexture);
      }

      bool on(const input::MouseButtonEvent& event) {
        // Only check button one for ui buttons
        if (event.button() != input::MouseButton::One) {
          return false;
        }
        auto manager = core::Core::Instance->get<ecs::EntityManager>();
        auto window = core::Core::Instance->get<platform::Platform>()->window();
        for (auto entity : manager->with({UiButtonComponent})) {
          auto button = entity.get<UiButton>(UiButtonComponent);
          auto frame = button->frame();
          auto pos = math::Vector2(
              event.position().x * window->scale(),
              window->height() - event.position().y * window->scale());
          if (
              pos.x >= frame.position().x && pos.x <= frame.position().x + frame.size().x &&
              pos.y >= frame.position().y && pos.y <= frame.position().y + frame.size().y) {
            if (event.type() == input::MouseButtonEvent::Type::press) {
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
}
