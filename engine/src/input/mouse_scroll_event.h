#pragma once

#include "math/vector.h"

#include "mouse_event.h"

namespace wage { namespace input {

  class MouseScrollEvent : public MouseEvent {

  public:
    MouseScrollEvent(math::Vector2 position) : MouseEvent(position) {
    }
  };

} }