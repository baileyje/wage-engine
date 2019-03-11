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
    UiButton(UiFrame frame) : UiButton(frame, Color::Clear) {
    }

    UiButton(UiFrame frame, Color color) : UiButton(frame, color, Texture::Default) {
    }

    UiButton(UiFrame frame, Texture texture) : UiButton(frame, Color::Clear, texture) {
    }

    UiButton(Color color = Color::Clear) : UiButton(UiFrame(), color) {
    }

    UiButton(Texture texture) : UiButton(UiFrame(), texture) {
    }

    UiButton(UiFrame frame, Color color, Texture texture) : UiComponent(frame), _color(color), _texture(texture) {
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

    inline void texture(Texture texture) {
      _texture = texture;
    }

  private:
    Color _color;

    Texture _texture;
  };
}

#endif //UI_BUTTON_H