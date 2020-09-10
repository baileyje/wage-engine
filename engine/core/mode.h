#pragma once

namespace wage {
  namespace core {
    enum class Mode {
      starting,
      running,
      paused,
      resetting,
      stopping,
      stopped
    };
  }
}