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
  
    Label(Frame frame, std::string text, Font font, Color color) : 
      UiComponent("Label", frame), text_(text), font_(font), color_(color) {    
    }

    Label(std::string text, Font font, Color color = Color(1, 1, 1, 1)) : 
      UiComponent("Label"), text_(text), font_(font), color_(color) {
    }

    virtual ~Label() {      
    }

    void update(ComponentContext* context) {
      Core::Instance->get<Renderer>()->renderText(transform()->position(), text(), font(), color());
    }

    inline std::string text() {
      return text_;
    }

    inline void set(std::string text) {
      text_ = text;
    }

    inline Font font() {
      return font_;
    }

    void setFont(Font font) {
      font_ = font;
    }

    inline Color color() {
      return color_;
    }

    inline void setColor(Color color) {
      color_ = color;
    }

  private:
  
    std::string text_;

    Font font_;

    Color color_;
  };

}

#endif //COMPONENT_UI_LABEL_H