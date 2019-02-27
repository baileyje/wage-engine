#ifndef PLATFORM_H
#define PLATFORM_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "core/service.h"
#include "platform/window.h"

namespace wage {

  class Platform : public Service {

  public:

    Platform() : Service("Platform") { }

    virtual ~Platform() { }

    void start();

    void stop();
    
    Window* window() {
      return _window;
    }

  private:

    Window* _window;
    
  };

}

#endif //PLATFORM_H