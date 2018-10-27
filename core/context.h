#ifndef CORE_CONTEXT_H
#define CORE_CONTEXT_H

#include <string>
#include <vector>

#include "fs/file_system.h"

#include "entity/scene.h"


class System;

class Context {

public:

  Context();

  virtual ~Context();

  // TODO:  I don't like systems being in the context.  How will we expose this so JSRT if needed? 
  virtual void add(System* system) = 0;

  virtual System* get(std::string name) = 0;

  // TODO: Should this be getters like lame sauce C++

  std::string rootPath;

  double timeStep;

  double time;

  double deltaTime;

  virtual void shouldStop() = 0;

  virtual Scene* getScene() = 0;

  virtual FileSystem* getFileSystem() = 0;

};

#endif // CORE_CONTEXT_H