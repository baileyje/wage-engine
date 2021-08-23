#pragma once

namespace wage {
  namespace core {
    enum class Mode {
      starting,
      running,
      paused,
      unpaused,
      resetting,
      stopping,
      stopped,
      none
    };
  }
}