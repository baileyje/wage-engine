#ifndef PLATFORM_H
#define PLATFORM_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "core/system.h"

namespace wage {

  class Platform : public System {

  public:

    Platform() : System("Platform") { }

    virtual ~Platform();

    LIFECYCLE_FUNC(init)

    LIFECYCLE_FUNC(deinit)
    
    GLFWwindow* getWindow() {
      return window;
    }

  private:

    GLFWwindow* window;
    
  };

}

#endif //PLATFORM_H