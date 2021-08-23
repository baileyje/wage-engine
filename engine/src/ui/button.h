#pragma once

#include <string>

#include "ui/component.h"
#include "core/core.h"
#include "render/renderer.h"

#define UiButtonComponent 501

namespace wage {
  namespace ui {

    class UiButton : public UiComponent {

    public:
      enum class State { released,
                         hover,
                         pressed };

      UiButton(UiFrame frame) : UiButton(frame, component::Color::Clear) {
      }

      UiButton(UiFrame frame, component::Color color) : UiButton(frame, color, render::TextureSpec()) {
      }

      UiButton(UiFrame frame, render::TextureSpec texture) : UiButton(frame, component::Color::Clear, texture, texture) {
      }

      UiButton(component::Color color = component::Color::Clear) : UiButton(UiFrame(), color) {
      }

      UiButton(render::TextureSpec texture) : UiButton(UiFrame(), texture) {
      }

      UiButton(UiFrame frame, component::Color color, render::TextureSpec texture) : UiComponent(frame), _color(color), _texture(texture), _pressedTexture(texture) {
      }

      UiButton(UiFrame frame, component::Color color, render::TextureSpec texture, render::TextureSpec pressedTexture) : UiComponent(frame), _color(color), _texture(texture), _pressedTexture(pressedTexture) {
      }

      virtual ~UiButton() {
      }

      inline component::Color color() const {
        return _color;
      }

      inline void color(component::Color color) {
        _color = color;
      }

      inline render::TextureSpec texture() const {
        return _texture;
      }

      inline render::TextureSpec pressedTexture() const {
        return _pressedTexture;
      }

      inline void pressedTexture(render::TextureSpec pressedTexture) {
        _pressedTexture = pressedTexture;
      }

      inline void texture(render::TextureSpec texture) {
        _texture = texture;
      }

      inline State state() const {
        return _state;
      }

      inline void state(State state) {
        _state = state;
      }

    private:
      component::Color _color;

      render::TextureSpec _texture;

      render::TextureSpec _pressedTexture;

      State _state = State::released;
    };

  }
}