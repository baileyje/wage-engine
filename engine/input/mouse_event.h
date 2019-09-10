#pragma once

#include "math/vector.h"

namespace wage { namespace input {

  class MouseEvent {

  public:
    MouseEvent(math::Vector2 position) : _position(position) {
    }

    inline math::Vector2 position() const {
      return _position;
    }

  protected:
    math::Vector2 _position;

  };

} }