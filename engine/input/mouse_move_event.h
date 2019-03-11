#ifndef INPUT_MOUSE_MOVE_EVENT_H
#define INPUT_MOUSE_MOVE_EVENT_H

#include "math/vector.h"

namespace wage {

  class MouseMoveEvent {

  public:
    MouseMoveEvent(Vector2 position) : _position(position) {
    }

    inline Vector2 position() {
      return _position;
    }

  private:
    Vector2 _position;
  };
}

#endif //INPUT_MOUSE_MOVE_EVENT_H
