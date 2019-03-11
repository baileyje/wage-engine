#include "input/input.h"

#include "core/core.h"
#include "platform/platform.h"

namespace wage {

  void Input::start() {
    messaging = Core::Instance->get<Messaging>();
    auto platform = Core::Instance->get<Platform>();
    window = platform->window();
  }
}
