#ifndef CORE_SYSTEM_CONTEXT_H
#define CORE_SYSTEM_CONTEXT_H

#include <string>
#include <vector>

#include "core/core.h"

namespace wage {

  class System;

  class SystemContext {

  public:

    virtual ~SystemContext() {}

    template <typename T>
    T* get() {
      return core->get<T>();
    }

    virtual double getTimeStep();

    virtual double getTime();

    virtual double getDeltaTime();

    virtual void shouldStop();

  private:

    SystemContext(Core* core) : core(core) {}

    Core* core;

    friend class Core;
  };

}

#endif // CORE_SYSTEM_CONTEXT_H