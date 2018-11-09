#include "core/context.h"

void Context::shouldStop() {
  core->shouldStop();
}

double Context::getTimeStep() {
  return core->getTimeStep();
}

double Context::getTime() {
  return core->getTime();
}

double Context::getDeltaTime() {
  return core->getDeltaTime();
}