#ifndef INPUT_KEY_EVENT_H
#define INPUT_KEY_EVENT_H

#include <initializer_list>

#include "input/key.h"
#include "input/key_modifier.h"

namespace wage {

  enum class KeyEventType {
    press,
    release,
    repeat
  };

  class KeyEvent {

  public:
    KeyEvent(Key key, KeyEventType type, int modifiers) : _key(key), _type(type), _modifiers(modifiers) {
    }

    inline Key key() {
      return _key;
    }

    inline KeyEventType type() {
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
      for (auto mod : mods) {
        all = all && set(mod);
      }
      return all;
    }

  private:
    Key _key;

    KeyEventType _type;

    int _modifiers;
  };
}

#endif //INPUT_KEY_EVENT_H
