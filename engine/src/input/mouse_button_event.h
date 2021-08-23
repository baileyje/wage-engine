#pragma once

#include <initializer_list>
#include "key_modifier.h"
#include "mouse_event.h"
#include "mouse_button.h"

namespace wage { namespace input {

  class MouseButtonEvent : public MouseEvent {

  public:
    enum class Type { press, release };

    MouseButtonEvent(MouseButton button, Type type, int modifiers, math::Vector2 position) : MouseEvent(position), _button(button), _type(type), _modifiers(modifiers) {
    }

    inline MouseButton button() const {
      return _button;
    }

    inline Type type() const {
      return _type;
    }

    inline int modifiers() const {
      return _modifiers;
    }

    bool set(KeyModifier mod) {
      int intMod = (int)mod;
      return (_modifiers & intMod) > 0;
    }

    bool set(std::initializer_list<KeyModifier> mods) {
      bool all = true;
      for (auto mod : mods) {
        all = all && set(mod);
      }
      return all;
    }

  private:

    MouseButton _button;

    Type _type;

    int _modifiers;
  };

} }