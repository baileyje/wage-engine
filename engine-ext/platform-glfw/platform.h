#ifndef GLFW_PLATFORM_H
#define GLFW_PLATFORM_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "platform/platform.h"

namespace wage {

  class GlfwPlatform : public Platform {

  public:

    GlfwPlatform() : Platform() { }

    virtual ~GlfwPlatform() { }

    void start() override;

    void stop() override;
    
  };

}

#endif //GLFW_PLATFORM_H