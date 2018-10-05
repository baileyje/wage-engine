#ifndef CORE_CONTEXT_H
#define CORE_CONTEXT_H

#include <string>
#include "core/core.h"

class Context {

public: 

  std::string rootPath;

  Core* core;

  double timeStep;

  double time;

  double deltaTime;

};

#endif // CORE_CONTEXT_H