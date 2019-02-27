#ifndef COMPONENT_UI_FRAME_H
#define COMPONENT_UI_FRAME_H

#include "math/vector.h"

namespace wage {

  class LayoutFrame {

  public:

    LayoutFrame() {}

    LayoutFrame(Vector2 position, Vector2 size) : _position(position), _size(size) {}

    virtual ~LayoutFrame() {}

    inline Vector2 position() {
      return _position;      
    }

    inline Vector2 size() {
      return _size;
    }

  private:

    Vector2 _position;

    Vector2 _size;

  };

}

#endif //COMPONENT_UI_FRAME_H