#pragma once

#include "core/service.h"
#include "platform/window.h"

namespace wage {
  namespace platform {

    /**
     * Platform represents the service for managing platform specific functionality. Mostly
     * just access to the window.
     */
    class Platform : public core::Service {

    public:
      Platform() : Service("Platform") {}

      virtual ~Platform() {}

      Window* window() {
        return _window;
      }

    protected:
      Window* _window;
    };
  }
}