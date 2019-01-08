#ifndef INPUT_MOUSE_SCROLL_EVENT_H
#define INPUT_MOUSE_SCROLL_EVENT_H

#include "math/vector.h"

namespace wage {

  class MouseScrollEvent {
  
  public:

    MouseScrollEvent(Vector2 position) : position_(position) {
    }

    inline Vector2 position() {
      return position_;
    }

  private:
    
    Vector2 position_;

  };

}

#endif //INPUT_MOUSE_SCROLL_EVENT_H