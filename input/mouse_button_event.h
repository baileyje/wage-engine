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

    MouseButtonEvent(int button, MouseButtonEventType type, int modifiers) : button_(button), type_(type), modifiers_(modifiers) {
    }

    inline int button() {
      return button_;
    }

    inline MouseButtonEventType type() {
      return type_;
    }

    inline int modifiers() {
      return modifiers_;
    }

    bool set(KeyModifier mod) {
      int intMod = (int)mod;
      return (modifiers_ & intMod) > 0;
    }

    bool set(std::initializer_list<KeyModifier> mods) {
      bool all = true;
      for(auto mod : mods) {
        all = all && set(mod);
      }
      return all;
    }

  private:
    
    int button_;

    MouseButtonEventType type_;

    int modifiers_;

  };

}

#endif //INPUT_MOUSE_BUTTON_EVENT_H
