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

    UiButton(UiFrame frame) : UiButton(frame, Color::Clear) {
    }

    UiButton(UiFrame frame, Color color) : UiButton(frame, color, Texture::Default) {
    }

    UiButton(UiFrame frame, Texture texture) : UiButton(frame, Color::Clear, texture, texture) {
    }

    UiButton(Color color = Color::Clear) : UiButton(UiFrame(), color) {
    }

    UiButton(Texture texture) : UiButton(UiFrame(), texture) {
    }

    UiButton(UiFrame frame, Color color, Texture texture) : UiComponent(frame), _color(color), _texture(texture), _pressedTexture(texture) {
    }

    UiButton(UiFrame frame, Color color, Texture texture, Texture pressedTexture) : UiComponent(frame), _color(color), _texture(texture), _pressedTexture(pressedTexture) {
    }

    virtual ~UiButton() {
    }

    inline Color color() const {
      return _color;
    }

    inline void color(Color color) {
      _color = color;
    }

    inline Texture texture() const {
      return _texture;
    }

    inline Texture pressedTexture() const {
      return _pressedTexture;
    }

    inline void pressedTexture(Texture pressedTexture) {
      _pressedTexture = pressedTexture;
    }

    inline void texture(Texture texture) {
      _texture = texture;
    }

    inline State state() const {
      return _state;
    }

    inline void state(State state) {
      _state = state;
    }

  private:
    Color _color;

    Texture _texture;

    Texture _pressedTexture;

    State _state = State::released;
  };
}

#endif //UI_BUTTON_H