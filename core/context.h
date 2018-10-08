#ifndef CORE_CONTEXT_H
#define CORE_CONTEXT_H

#include <string>

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
};

#endif // CORE_CONTEXT_H