#pragma once

#include "core/service.h"
#include "platform/window.h"

namespace wage { namespace platform {

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

} }