#ifndef INPUT_KEY_PRESS_H
#define INPUT_KEY_PRESS_H

namespace wage {

  // TODO: Enum for keys

  enum class KeyEventType {
    press, release, repeat
  };

  class KeyEvent {
  
  public:

    KeyEvent(int key, KeyEventType type) : key_(key), type_(type) {
    }

    inline int key() {
      return key_;
    }

    inline KeyEventType type() {
      return type_;
    }

  private:
    
    int key_;

    KeyEventType type_;

  };

}

#endif //INPUT_KEY_PRESS_H
