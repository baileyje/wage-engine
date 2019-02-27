#ifndef CORE_SYSTEM_CONTEXT_H
#define CORE_SYSTEM_CONTEXT_H

#include <string>
#include <vector>

#include "core/core.h"

namespace wage {

  class System;

  class SystemContext {

  public:

    SystemContext(Core* core) : core(core) {}

    virtual ~SystemContext() {}

    template <typename T>
    T* get() {
      return core->get<T>();
    }

    virtual double timeStep();

    virtual double time();

    virtual double deltaTime();

    virtual void shouldStop();

  private:

    Core* core;

    friend class Core;
  };

}

#endif // CORE_SYSTEM_CONTEXT_H