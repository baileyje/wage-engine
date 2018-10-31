#ifndef PLATFORM_H
#define PLATFORM_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "core/system.h"

class Platform : public System {

public:

  Platform() : System("platform") { }

  virtual ~Platform();

  LIFECYCLE_FUNC(init)

  LIFECYCLE_FUNC(deinit)
  
  GLFWwindow* getWindow() {
    return window;
  }

private:

  GLFWwindow* window;
  
};

#endif //PLATFORM_H