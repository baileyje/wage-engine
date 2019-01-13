#include "core/system/context.h"

namespace wage {

  void SystemContext::shouldStop() {
    core->shouldStop();
  }

  double SystemContext::timeStep() {
    return core->timeStep();
  }

  double SystemContext::time() {
    return core->time();
  }

  double SystemContext::deltaTime() {
    return core->deltaTime();
  }

}