#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <string>

#include "ui/component.h"
#include "render/font.h"
#include "core/core.h"
#include "render/renderer.h"

namespace wage {

  class UiButton : public UiComponent {

  public:

    enum class State { released, hover, pressed };

    UiButton(UiFrame frame) : UiButton(frame, math::Color::Clear) {
    }

    UiButton(UiFrame frame, math::Color color) : UiButton(frame, color, render::Texture::Default) {
    }

    UiButton(UiFrame frame, render::Texture texture) : UiButton(frame, math::Color::Clear, texture, texture) {
    }

    UiButton(math::Color color = math::Color::Clear) : UiButton(UiFrame(), color) {
    }

    UiButton(render::Texture texture) : UiButton(UiFrame(), texture) {
    }

    UiButton(UiFrame frame, math::Color color, render::Texture texture) : UiComponent(frame), _color(color), _texture(texture), _pressedTexture(texture) {
    }

    UiButton(UiFrame frame, math::Color color, render::Texture texture, render::Texture pressedTexture) : UiComponent(frame), _color(color), _texture(texture), _pressedTexture(pressedTexture) {
    }

    virtual ~UiButton() {
    }

    inline math::Color color() const {
      return _color;
    }

    inline void color(math::Color color) {
      _color = color;
    }

    inline render::Texture texture() const {
      return _texture;
    }

    inline render::Texture pressedTexture() const {
      return _pressedTexture;
    }

    inline void pressedTexture(render::Texture pressedTexture) {
      _pressedTexture = pressedTexture;
    }

    inline void texture(render::Texture texture) {
      _texture = texture;
    }

    inline State state() const {
      return _state;
    }

    inline void state(State state) {
      _state = state;
    }

  private:
    math::Color _color;

    render::Texture _texture;

    render::Texture _pressedTexture;

    State _state = State::released;
  };
}

#endif //UI_BUTTON_H