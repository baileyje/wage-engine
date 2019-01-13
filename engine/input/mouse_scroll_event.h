#ifndef INPUT_MOUSE_SCROLL_EVENT_H
#define INPUT_MOUSE_SCROLL_EVENT_H

#include "math/vector.h"

namespace wage {

  class MouseScrollEvent {
  
  public:

    MouseScrollEvent(Vector2 position) : _position(position) {
    }

    inline Vector2 position() {
      return _position;
    }

  private:
    
    Vector2 _position;

  };

}

#endif //INPUT_MOUSE_SCROLL_EVENT_H
