#ifndef INPUT_MOUSE_BUTTON_EVENT_H
#define INPUT_MOUSE_BUTTON_EVENT_H

#include <initializer_list>
#include "input/key_modifier.h"

namespace wage {

  enum class MouseButtonEventType {
    press, release
  };

  class MouseButtonEvent {
  
  public:

    MouseButtonEvent(int button, MouseButtonEventType type, int modifiers) : _button(button), _type(type), _modifiers(modifiers) {
    }

    inline int button() {
      return _button;
    }

    inline MouseButtonEventType type() {
      return _type;
    }

    inline int modifiers() {
      return _modifiers;
    }

    bool set(KeyModifier mod) {
      int intMod = (int)mod;
      return (_modifiers & intMod) > 0;
    }

    bool set(std::initializer_list<KeyModifier> mods) {
      bool all = true;
      for(auto mod : mods) {
        all = all && set(mod);
      }
      return all;
    }

  private:
    
    int _button;

    MouseButtonEventType _type;

    int _modifiers;

  };

}

#endif //INPUT_MOUSE_BUTTON_EVENT_H
