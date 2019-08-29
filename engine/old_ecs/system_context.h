#ifndef CORE_SYSTEM_CONTEXT_H
#define CORE_SYSTEM_CONTEXT_H

#include <string>
#include <vector>

#include "core/frame.h"
#include "old_ecs/entity_manager.h"

namespace wage {

  class System;

  class SystemContext {

  public:
    SystemContext(const EntityManager* entityManager, const Frame& frame) : _entityManager(entityManager), frame(frame) {}

    virtual ~SystemContext() {}

    inline double timeStep() const {
      return frame.timeStep();
    }

    inline double time() const {
      return frame.time();
    }

    inline double deltaTime() const {
      return frame.deltaTime();
    }

    inline const EntityManager* entityManager() const {
      return _entityManager;
    }

  private:
    const EntityManager* _entityManager;

    const Frame frame;
  };
}

#endif // CORE_SYSTEM_CONTEXT_H