#ifndef INPUT_MOUSE_BUTTON_EVENT_H
#define INPUT_MOUSE_BUTTON_EVENT_H

#include <initializer_list>
#include "key_modifier.h"
#include "mouse_event.h"

namespace wage {

  class MouseButtonEvent : public MouseEvent {

  public:
    enum class Type { press, release };

    MouseButtonEvent(int button, Type type, int modifiers, Vector2 position) : MouseEvent(position), _button(button), _type(type), _modifiers(modifiers) {
    }

    inline int button() const {
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
    int _button;

    Type _type;

    int _modifiers;
  };
}

#endif //INPUT_MOUSE_BUTTON_EVENT_H
