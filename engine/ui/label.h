#pragma once

#include <string>

#include "ui/component.h"
#include "render/font.h"
#include "core/core.h"
#include "render/renderer.h"

#define UiLabelComponent 500

namespace wage {
  namespace ui {

    class UiLabel : public UiComponent {

    public:
      UiLabel() : UiComponent() {
      }

      UiLabel(std::string text, render::Font font, math::Color color = math::Color::White) : UiLabel(UiFrame(), text, font, color) {
      }

      UiLabel(UiFrame frame, std::string text, render::Font font, math::Color color) : UiComponent(frame), _text(text), _font(font), _color(color) {
      }

      virtual ~UiLabel() {
      }

      inline std::string text() const {
        return _text;
      }

      inline void set(std::string text) {
        _text = text;
      }

      inline render::Font font() const {
        return _font;
      }

      void font(render::Font font) {
        _font = font;
      }

      inline math::Color color() const {
        return _color;
      }

      inline void color(math::Color color) {
        _color = color;
      }

    private:
      std::string _text;

      render::Font _font;

      math::Color _color;
    };

  }
}