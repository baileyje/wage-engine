#ifndef INPUT_MOUSE_EVENT_H
#define INPUT_MOUSE_EVENT_H

#include "math/vector.h"

namespace wage {

  class MouseEvent {

  public:
    MouseEvent(Vector2 position) : _position(position) {
    }

    inline Vector2 position() const {
      return _position;
    }

  protected:
    Vector2 _position;
  };
}

#endif //INPUT_MOUSE_EVENT_H
