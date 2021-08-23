#pragma once

#include "math/vector.h"

#include "mouse_event.h"

namespace wage { namespace input {

  class MouseMoveEvent : public MouseEvent {

  public:
    MouseMoveEvent(math::Vector2 position) : MouseEvent(position) {
    }
  };

} }
