#ifndef COMPONENT_UI_FRAME_H
#define COMPONENT_UI_FRAME_H

#include "math/vector.h"

namespace wage {

  class UiFrame {

  public:
    UiFrame() {}

    UiFrame(math::Vector2 position, math::Vector2 size) : _position(position), _size(size) {}

    virtual ~UiFrame() {}

    inline math::Vector2 position() const {
      return _position;
    }

    inline void position(math::Vector2 position) {
      _position = position;
    }

    inline math::Vector2 size() const {
      return _size;
    }

    inline void size(math::Vector2 size) {
      _size = size;
    }

  private:
    math::Vector2 _position;

    math::Vector2 _size;
  };
}

#endif //COMPONENT_UI_FRAME_H