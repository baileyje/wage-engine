#ifndef INPUT_KEY_EVENT_H
#define INPUT_KEY_EVENT_H

#include <initializer_list>

#include "engine/input/key.h"
#include "engine/input/key_modifier.h"

namespace wage {

  enum class KeyEventType {
    press, release, repeat
  };

  class KeyEvent {
  
  public:

    KeyEvent(Key key, KeyEventType type, int modifiers) : key_(key), type_(type), modifiers_(modifiers) {
    }

    inline Key key() {
      return key_;
    }

    inline KeyEventType type() {
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
    
    Key key_;

    KeyEventType type_;

    int modifiers_;

  };

}

#endif //INPUT_KEY_EVENT_H
