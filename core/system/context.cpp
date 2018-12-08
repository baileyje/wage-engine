#include "core/system/context.h"
namespace wage {

  void SystemContext::shouldStop() {
    core->shouldStop();
  }

  double SystemContext::getTimeStep() {
    return core->getTimeStep();
  }

  double SystemContext::getTime() {
    return core->getTime();
  }

  double SystemContext::getDeltaTime() {
    return core->getDeltaTime();
  }

}