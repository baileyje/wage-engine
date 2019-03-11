#ifndef INPUT_MOUSE_SCROLL_EVENT_H
#define INPUT_MOUSE_SCROLL_EVENT_H

#include "math/vector.h"

#include "mouse_event.h"

namespace wage {

  class MouseScrollEvent : public MouseEvent {

  public:
    MouseScrollEvent(Vector2 position) : MouseEvent(position) {
    }
  };
}

#endif //INPUT_MOUSE_SCROLL_EVENT_H
