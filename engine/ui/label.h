#ifndef COMPONENT_UI_LABEL_H
#define COMPONENT_UI_LABEL_H

#include <string>

#include "ui/component.h"
#include "render/font.h"
#include "core/core.h"
#include "render/renderer.h"

namespace wage {

  class Label : public UiComponent {
    
  public:

    Label() : UiComponent("Label") {
    }

    Label(LayoutFrame frame, std::string text, Font font, Color color) : 
      UiComponent("Label", frame), _text(text), _font(font), _color(color) {    
    }

    Label(std::string text, Font font, Color color = Color(1, 1, 1, 1)) : 
      UiComponent("Label"), _text(text), _font(font), _color(color) {
    }

    virtual ~Label() {      
    }

    inline std::string text() {
      return _text;
    }

    inline void set(std::string text) {
      _text = text;
    }

    inline Font font() {
      return _font;
    }

    void font(Font font) {
      _font = font;
    }

    inline Color color() {
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