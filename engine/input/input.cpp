#include "input/input.h"

#include "core/core.h"
#include "platform/platform.h"

namespace wage { namespace input {

  void Input::start() {
    messaging = core::Core::Instance->get<messaging::Messaging>();
    auto platform = core::Core::Instance->get<platform::Platform>();
    window = platform->window();
  }

} }