#include "core/context.h"

void Context::shouldStop() {
  core->shouldStop();
}

Scene* Context::getScene() {
  return core->getScene();
}

FileSystem* Context::getFileSystem() {
  return core->getFileSystem();
}

std::string Context::getRootPath() {
  return core->getRootPath();
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