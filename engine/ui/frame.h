#ifndef COMPONENT_UI_FRAME_H
#define COMPONENT_UI_FRAME_H

#include "engine/math/vector.h"

namespace wage {

  class Frame {

  public:

    Frame() {}

    Frame(Vector2 position, Vector2 size) : position_(position), size_(size) {}

    virtual ~Frame() {}

    inline Vector2 position() {
      return position_;      
    }

    inline Vector2 size() {
      return size_;
    }

  private:

    Vector2 position_;

    Vector2 size_;

  };

}

#endif //COMPONENT_UI_FRAME_H