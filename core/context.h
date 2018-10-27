#ifndef CORE_CONTEXT_H
#define CORE_CONTEXT_H

#include <string>
#include <vector>

#include "fs/file_system.h"

#include "entity/entity.h"
#include "entity/store.h"
#include "entity/component.h"


class System;

class Context {

public:

  Context();

  virtual ~Context();

  virtual void add(System* system) = 0;

  virtual System* get(std::string name) = 0;

  // TODO: Should this be getters like lame sauce C++

  std::string rootPath;

  double timeStep;

  double time;

  double deltaTime;

  virtual void shouldStop() = 0;

  virtual void add(Entity* entity) = 0;

  virtual Entity* getCamera() = 0;

  virtual EntityStore* getEntities() = 0;

  virtual FileSystem* getFileSystem() = 0;

};

#endif // CORE_CONTEXT_H