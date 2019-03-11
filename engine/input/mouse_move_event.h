#ifndef INPUT_MOUSE_MOVE_EVENT_H
#define INPUT_MOUSE_MOVE_EVENT_H

#include "math/vector.h"

#include "mouse_event.h"

namespace wage {

  class MouseMoveEvent : public MouseEvent {

  public:
    MouseMoveEvent(Vector2 position) : MouseEvent(position) {
    }
  };
}

#endif //INPUT_MOUSE_MOVE_EVENT_H
