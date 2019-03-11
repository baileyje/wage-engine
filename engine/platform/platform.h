#ifndef PLATFORM_H
#define PLATFORM_H

#include "core/service.h"
#include "platform/window.h"

namespace wage {

  class Platform : public Service {

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

#endif //PLATFORM_H