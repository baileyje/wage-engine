#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "platform/platform.h"

namespace wage {
  namespace platform {

    class GlfwPlatform : public Platform {

    public:
      GlfwPlatform() : Platform() {}

      virtual ~GlfwPlatform() {}

      void start() override;

      void stop() override;

      virtual void flushEvents() override {
        glfwPollEvents();
      }
    };

  }
}