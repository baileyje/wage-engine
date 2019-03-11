#ifndef COMPONENT_UI_LABEL_H
#define COMPONENT_UI_LABEL_H

#include <string>

#include "ui/component.h"
#include "render/font.h"
#include "core/core.h"
#include "render/renderer.h"

namespace wage {

  class UiLabel : public UiComponent {

  public:
    UiLabel() : UiComponent() {
    }

    UiLabel(std::string text, Font font, Color color = Color::White) : UiLabel(UiFrame(), text, font, color) {
    }

    UiLabel(UiFrame frame, std::string text, Font font, Color color) : UiComponent(frame), _text(text), _font(font), _color(color) {
    }

    virtual ~UiLabel() {
    }

    inline std::string text() const {
      return _text;
    }

    inline void set(std::string text) {
      _text = text;
    }

    inline Font font() const {
      return _font;
    }

    void font(Font font) {
      _font = font;
    }

    inline Color color() const {
      return _color;
    }

    inline void color(Color color) {
      _color = color;
    }

  private:
    std::string _text;

    Font _font;

    Color _color;
  };
}

#endif //COMPONENT_UI_LABEL_H