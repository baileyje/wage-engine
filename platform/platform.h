#ifndef PLATFORM_H
#define PLATFORM_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "core/system.h"

#include "platform/window.h"

namespace wage {

  class Platform : public System {

  public:

    Platform() : System("Platform") { }

    virtual ~Platform() { }

    LIFECYCLE_FUNC(init)

    LIFECYCLE_FUNC(deinit)
    
    Window* getWindow() {
      return window;
    }

  private:

    Window* window;
    
  };

}

#endif //PLATFORM_H