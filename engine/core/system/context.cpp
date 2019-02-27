#include "core/system/context.h"

namespace wage {

  void SystemContext::shouldStop() {
    core->shouldStop();
  }

  double SystemContext::timeStep() {
    return core->frame().timeStep();
  }

  double SystemContext::time() {
    return core->frame().time();
  }

  double SystemContext::deltaTime() {
    return core->frame().deltaTime();
  }

}