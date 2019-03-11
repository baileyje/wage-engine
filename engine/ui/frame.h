#ifndef COMPONENT_UI_FRAME_H
#define COMPONENT_UI_FRAME_H

#include "math/vector.h"

namespace wage {

  class UiFrame {

  public:
    UiFrame() {}

    UiFrame(Vector2 position, Vector2 size) : _position(position), _size(size) {}

    virtual ~UiFrame() {}

    inline Vector2 position() const {
      return _position;
    }

    inline void position(Vector2 position) {
      _position = position;
    }

    inline Vector2 size() const {
      return _size;
    }

    inline void size(Vector2 size) {
      _size = size;
    }

  private:
    Vector2 _position;

    Vector2 _size;
  };
}

#endif //COMPONENT_UI_FRAME_H