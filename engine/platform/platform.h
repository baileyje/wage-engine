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
    
    Window* window() {
      return window_;
    }

  private:

    Window* window_;
    
  };

}

#endif //PLATFORM_H